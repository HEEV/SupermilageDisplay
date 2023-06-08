// Compile like g++ ArduinoDriver.cpp -o ArduinoDriver.o -lusb-1.0 [Outside of full project]

// Runs transfers in async mode, and has hotplug support
#include "Serial/ArduinoDriver.h"

#include <stdlib.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>

#ifdef WIN32
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

//TODO: Remove include
#include <CommunicationManager.h>
#include <Packets.h>

using namespace std::chrono_literals;

// #define DEBUG_TO_TERMINAL
// #define DEBUG_TO_CB
#define DEBUG_CTRL_CB
#define DEBUG_SEND_CB
#define DEBUG_RECV_CB

// --------------------------------------------------------------------
// Once this is migrated fully to USB.cpp this will be unnec
// Specifies contexts, handles, and separate namespaces for each device 
// --------------------------------------------------------------------

enum DEVICE
{
    CH34x_type,
    ARDUINO_type
};

namespace CH34x
{
// VENDOR DEFINED ATTACH (Also we init serial slightly differently)
	// ch34x_vendor_read( VENDOR_VERSION, 0x0000, 0x0000,
	// 		serial, buf, 0x02 ); --- Dont do this 
	// ch34x_vendor_write( VENDOR_SERIAL_INIT, 0x0000, 0x0000, // Used in init
	// 		serial, NULL, 0x00 );
	// ch34x_vendor_write( VENDOR_WRITE, 0x1312, 0xD982, // Used for baudrate
	// 		serial, NULL, 0x00 );
	// ch34x_vendor_write( VENDOR_WRITE, 0x0F2C, 0x0004, // Used for baudrate
	// 		serial, NULL, 0x00 );
	// ch34x_vendor_read( VENDOR_READ, 0x2518, 0x0000,
	// 		serial, buf, 0x02 );
// --- We dont use this ---
	// ch34x_vendor_write( VENDOR_WRITE, 0x2727, 0x0000,
	// 		serial, NULL, 0x00 );
	// ch34x_vendor_write( VENDOR_MODEM_OUT, 0x009F, 0x0000,
	// 		serial, NULL, 0x00 );
// Vendor defined -> see ch34x.c driver
    enum
    {
        VENDOR_READ				= 0x95,  // Unused
        VENDOR_WRITE			= 0x9A,
        VENDOR_SERIAL_INIT		= 0xA1,
        VENDOR_MODEM_OUT		= 0xA4,  // Handshake
        VENDOR_VERSION			= 0x5F,  // Unused
    };
    uint8_t EP_DATA_IN = (0x2 | LIBUSB_ENDPOINT_IN);
    uint8_t EP_DATA_OUT = (0x2 | LIBUSB_ENDPOINT_OUT);
    uint8_t CTRL_OUT = (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT);
    int NUM_INTERFACES = 1;
    int DEFAULT_BAUD_RATE = 9600;
    uint8_t dtr = 0;
    uint8_t rts = 0;
    uint16_t vendorID = 0x1A86;
    uint16_t productID = 0x7523;
} // namespace CH34x

namespace ARDUINO
{
    enum
    {
        VENDOR_SERIAL_INIT		= 0x22,
        VENDOR_WRITE			= 0x20
    };
    uint8_t EP_DATA_IN = 0x83;
    uint8_t EP_DATA_OUT = 0x02;
    uint8_t CTRL_OUT = 0x21;
    int NUM_INTERFACES = 1;
    int DEFAULT_BAUD_RATE = 9600;
    uint8_t dtr = 0x01;
    uint8_t rts = 0x02;
    uint16_t vendorID = 0x239A;
    uint16_t productID = 0x800C;
} // namespace ARDUINO

static struct libusb_device_handle *devh = NULL;
static struct libusb_context * hp_ctx = NULL;
static struct libusb_context * gen_ctx = NULL;
static struct libusb_transfer *recv_bulk_transfer = NULL;
static struct libusb_transfer *send_bulk_transfer = NULL;

#ifdef DEBUG_TO_TERMINAL
int runSetup = 0;
#endif

uint8_t do_exit = 0;
uint8_t ctrl_exit = 0;
uint8_t did_recv = 0;
uint8_t did_send = 0;
uint8_t hp_done = 0;

uint8_t recvbuf[256];
constexpr int SEND_BUFFER_SIZE = 64;

unsigned char SEND_TEXT[] = "HELLO WORLD!";

// --------------------------------------------------------------------
// --------------------------------------------------------------------

/**
 * This is the callback that is handled to send async control transfers
 * 
 * @param transfer The libusb transfer that will be sent as a control transfer
*/
static void LIBUSB_CALL ctrl_setup_cb(struct libusb_transfer *transfer)
{
    if (ctrl_exit == 2)
        return; // It really likes to add packets after the init finished
    if (transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        ctrl_exit = 0;
        libusb_free_transfer(transfer);
        return;
    }

    #ifdef DEBUG_CTRL_CB
    printf("async cb_mode_changed length=%d actual_length=%d\n",
		transfer->length, transfer->actual_length);
    std::vector<unsigned char> test;
    test.reserve(transfer->length);
    memcpy(&test[0], transfer->buffer, transfer->length);
    for (int i = 0; i < transfer->length; ++i)
    {
        printf("%.2X ", test[i]);
    }
    printf("\n");
    #endif

    int r = libusb_submit_transfer(transfer);
    if (r < 0)
    {
        //fprintf(stderr, "LIBUSB_SUBMIT_TRANSFER_ERROR ERROR: %s   :   ctrl_setup_cb 2\n", libusb_error_name(r));
        ctrl_exit = 0;
    }
    else
        ctrl_exit = 2;
}

/**
 * This is a helper function to abstract sending a async control transfer to the same
 * parameters that a sync control transfer uses, it also handles the callback
 * 
 * @param transfer The libusb transfer that will be sent as a control transfer
 * @param bmRequest The control endpoint for the device (ex. CTRL_OUT)
 * @param bRequest The request type, usually a specific command, related to control transfers for the device (ex. SERIAL_WRITE)
 * @param wValue The byte code that is sent to the device
 * @param wIndex The address the byte code will be sent to for the device
 * @param ctx The context the control transfer should be handled within
 * @param wLength The number of bytes to transfer, specifically in the user_data field (used in ATmega32u4)
 * @param data The user_data field (used in ATmega32u4)
*/
int ctrl_transfer (libusb_transfer * transfer, uint8_t bmRequest, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, libusb_context * ctx = NULL, uint16_t wLength = 0, unsigned char * data = NULL)
{
    unsigned char buf[LIBUSB_CONTROL_SETUP_SIZE + 1];
    transfer = libusb_alloc_transfer(0);

	libusb_fill_control_setup(buf, bmRequest, bRequest, wValue, wIndex, wLength);

    #ifdef DEBUG_TO_TERMINAL
    runSetup++;
	printf("[%d] Buffer Before Sent:", runSetup);
    for (int i = 0; i < sizeof(buf); ++i)
    {
        printf("%.2X ", buf[i]);
    }
    printf("\n");
    #endif
    
    libusb_fill_control_transfer(transfer, devh, buf, ctrl_setup_cb, data, 1000);

	int r = libusb_submit_transfer(transfer);
    if (r < 0)
    {
        return r;
    }

    while (!ctrl_exit)
    {
        // Receive data callback
        struct timeval tv = {0L, 0L};
        r = libusb_handle_events_timeout(ctx, &tv);
        if (r < 0)
        {
            printf("Failed to Handle\n");
            libusb_free_transfer(transfer);
            return -1;
        }
    }

    //ctrl_exit = 0; // DO THIS AFTER CALLING IT

    return 0;
}

/**
 * This is a helper function that initializes the baudrate for ch34x family devices, it is used in safe_init_ch34x
 * 
 * @param transfer The libusb transfer that will be sent as a control transfer
 * @param buadRate The baudrate that should be used for the device
 * @param ctx The context the control transfer should be handled within
*/
int init_baudrate_ch34x(struct libusb_transfer * transfer, int baudRate, libusb_context * ctx = NULL)
{
    static int baudEncoding[] = {2400, 0xd901, 0x0038, 4800, 0x6402,
                             0x001f, 9600, 0xb202, 0x0013, 19200, 0xd902, 0x000d, 38400,
                             0x6403, 0x000a, 115200, 0xcc03, 0x0008};
    int r;

    for (int i = 0; i < sizeof(baudEncoding) / sizeof(int) / 3; i++)
    {
        if (baudEncoding[i * 3] == baudRate)
        {
            // Vendor looks to be using something close 19200, which leads to different encoding
            //int r = libusb_control_transfer(devh, CTRL_OUT, 0x9a, 0x1312, baudEncoding[i * 3 + 1], NULL, 0, 1000);
	        r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_WRITE, 0x1312, baudEncoding[i * 3 + 1], ctx);
            if (r < 0)
            {
                fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x1312\n");
                return r;
            }
            ctrl_exit = 0;

            //r = libusb_control_transfer(devh, CTRL_OUT, 0x9a, 0x0f2c, baudEncoding[i * 3 + 2], NULL, 0, 1000);
	        r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_WRITE, 0x0f2c, baudEncoding[i * 3 + 2], ctx);
            if (r < 0)
            {
                fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x0f2c\n");
                return r;
            }
            ctrl_exit = 0;

            return 0;
        }
    }
    fprintf(stderr, "Unsupported baudrate\n");
    return -1;
}

/**
 * This is a function that initializes the ch34x family devices, by sending all required control transfers
 * 
 * @param buadRate The baudrate that should be used for the device
 * @param ctx The context the control transfer should be handled within
*/
int safe_init_ch34x(int baudRate, libusb_context * ctx = NULL)
{
    int r;
    unsigned char buf[LIBUSB_CONTROL_SETUP_SIZE + 1];
	struct libusb_transfer *transfer;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0xa1, 0, 0, NULL, 0, 1000);
    r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_SERIAL_INIT, 0, 0, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_SERIAL_INIT\n");
        return r;
    }

    ctrl_exit = 0;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0x9a, 0x2518, 0x0050, NULL, 0, 1000);
    r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_WRITE, 0x2518, 0x0050, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x2518\n");
        return r;
    }

    ctrl_exit = 0;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0xa1, 0x501f, 0xd90a, NULL, 0, 1000);
	r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_SERIAL_INIT, 0x501f, 0xd90a, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_SERIAL_INIT,0x501f\n");
        return r;
    }

    ctrl_exit = 0;
    
    // --- BAUDRATE ---
    r = init_baudrate_ch34x(transfer, baudRate, ctx);
    if (r < 0)
        return r;
    // Does ctrl_exit = 0; inside init_baudrate_ch34x()

    // --- HANDSHAKE ---
    //if (libusb_control_transfer(devh, CTRL_OUT, 0xa4, ~((dtr ? 1 << 5 : 0) | (rts ? 1 << 6 : 0)), 0, NULL, 0, 1000) < 0)
	r = ctrl_transfer(transfer, CH34x::CTRL_OUT, CH34x::VENDOR_MODEM_OUT, ~((CH34x::dtr ? 1 << 5 : 0) | (CH34x::rts ? 1 << 6 : 0)), 0, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed to complete handshake [Handshake byte not set]\n");
        return r;
    }

    //ctrl_exit = 0; // Last init transfer should not set ctrl_exit to 0

    #ifdef DEBUG_TO_TERMINAL
    printf("Finished safe_init_ch34x\n");
    #endif

    return r;
}

/**
 * This is a function that initializes the ATmega32u4 family devices, by sending all required control transfers
 * (Has different control transfers than the ch34x family)
 * 
 * @param buadRate The baudrate that should be used for the device
 * @param ctx The context the control transfer should be handled within
*/
int safe_init_ATmega32u4(int baudRate, libusb_context * ctx = NULL)
{
    int r;
    unsigned char buf[LIBUSB_CONTROL_SETUP_SIZE + 1];
	struct libusb_transfer *transfer;

    // r = libusb_control_transfer(devh, CTRL_OUT, VENDOR_SERIAL_INIT, ACM_CTRL_DTR | ACM_CTRL_RTS, 0, NULL, 0, 0);
    r = ctrl_transfer(transfer, ARDUINO::CTRL_OUT, ARDUINO::VENDOR_SERIAL_INIT, (ARDUINO::dtr | ARDUINO::rts), 0, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_SERIAL_INIT\n");
        return r;
    }

    ctrl_exit = 0;


    unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 };
    // r = libusb_control_transfer(devh, CTRL_OUT, VENDOR_WRITE, 0, 0, encoding, sizeof(encoding), 0);
    r = ctrl_transfer(transfer, ARDUINO::CTRL_OUT, ARDUINO::VENDOR_WRITE, 0, 0, ctx, sizeof(encoding), encoding);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x2518\n");
        return r;
    }

    ctrl_exit = 0;

    #ifdef DEBUG_TO_TERMINAL
    printf("Finished safe_init_ATmega32u4\n");
    #endif

    return r;
}

/**
 * This is the callback that is handled to send async bulk transfers used to recieve from the device
 * In this callback all data is recieved by the program
 * ex. If we receive sensor data we upload it to FAST-DDS within this callback
 * 
 * @param transfer The libusb transfer that will be received as a bulk transfer
*/
static void LIBUSB_CALL recv_cb(struct libusb_transfer *transfer)
{
    if (do_exit == 2 | did_recv == 2)
        return;
    if (transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        printf("LIBUSB_TRANSFER_COMPLETED FAILED, %s\n", libusb_error_name(transfer->status) );
        did_recv = 2;
        libusb_free_transfer(transfer);
        return;
    }

    int r = libusb_submit_transfer(recv_bulk_transfer);
    if (r < 0)
    {
        fprintf(stderr, "LIBUSB_TRANSFER_ERROR ERROR: %s   :   recv_cb 2\n", libusb_error_name(r));
        did_recv = 0;
    }
    else
    {
        did_recv = 2;

        // TODO: Remove Following Code
        #pragma pack(push, 1)
        struct Data
        {
            int16_t sensor;
            float data;
            int64_t time;
        };
        #pragma pack(pop)
        Data* data = &((Data*)recvbuf)[0];
        WheelData wd;
        WindSpeed ws;
        EngineTemp et;
        CarTilt ct;
        printf("%d : ", data->sensor);
        printf("%f : ", data->data);
        printf("%ld\n", data->time);
        ComData* comData = (ComData*)(transfer->user_data);
        auto& man = comData->man;
        switch(data->sensor)
        {
        case 0:
            wd.velocity(data->data);
            wd.distTravelled(data->time * data->data);
            wd.head().timeOcc(data->time);
            man->writeData(comData->wheelID, &wd);
            break;
        case 1:
            ws.headSpeed(data->data);
            ws.head().timeOcc(data->time);
            man->writeData(comData->windID, &ws);
            break;
        case 2:
            et.temp(data->data);
            et.head().timeOcc(data->time);
            man->writeData(comData->engID, &et);
            break;
        case 3:
            ct.angle(data->data);
            ct.head().timeOcc(data->time);
            man->writeData(comData->tiltID, &ct);
            break;
        }

        // ------------------------

        #ifdef DEBUG_RECV_CB
        printf("Bytes recv: %d    ", transfer->actual_length);
        printf("Data callback[");
        for (int i = 0; i < transfer->actual_length; ++i)
        {
            // printf("%.2X ", recvbuf[i]);
            putchar(recvbuf[i]);
            printf(" ");

        }
        #endif
    }
}

/**
 * This is a helper function to abstract receiving data in an async manner
 * This sends the bulk transfer in a receive format and handles the receive callback
 * 
 * @param endpoint This is the input endpoint of the device, because it is received by libusb (ex. EP_DATA_IN, `lsusb -vv`-> bEndpointAddress     0x83  EP 3 IN)
 * @param ctx The context the control transfer should be handled within
 * @param data A pointer to ComData, a struct which has the communication manager, as well as sensor id's
*/
int recvData(uint8_t endpoint, libusb_context * ctx = NULL, ComData* data = nullptr)
{
    int r;
    #ifdef DEBUG_TO_CB
    printf("In recvData loop\n");
    printf("Recv Endpoint: %d\n",endpoint);
    #endif
    recv_bulk_transfer = libusb_alloc_transfer(0);
    if (!recv_bulk_transfer)
    {
        fprintf(stderr, "LIBUSB_ALLOC_TRANSFER ERROR\n");
        return -1;
    }
    printf("EP_DATA_IN: %d\n", endpoint);


    libusb_fill_bulk_transfer(recv_bulk_transfer, devh, endpoint, recvbuf,
                            sizeof(recvbuf), recv_cb, data, 1000);

    r = libusb_submit_transfer(recv_bulk_transfer);
    if (r < 0)
    {
        // Really only fails when you unplug, ie hotplug picks up slack
        //fprintf(stderr, "LIBUSB_SUBMIT_TRANSFER ERROR: %s   :   recvData 1\n", libusb_error_name(r));
        libusb_free_transfer(recv_bulk_transfer);
        return -1;
    }

    #ifdef DEBUG_TO_CB
    printf("Before handle = do_exit[%d] ; did_recv[%d]\n", do_exit, did_recv);
    #endif
    while (!did_recv)
    {
        // Receive data callback
        struct timeval tv = {0L, 0L};
        r = libusb_handle_events_timeout(ctx, &tv);
        if (r < 0)
        {
            libusb_free_transfer(recv_bulk_transfer);
            return -1;
        }
    }
    #ifdef DEBUG_TO_CB
    printf("recvData : Exited the loop\n");
    #endif
    return 0;
}

/**
 * This is the callback that is handled to send async send transfers used to send to the device
 * In this callback all data is sent by the program to the device
 * ex. We could send commands to the board or send say rf packets
 * 
 * @param transfer The libusb transfer that will be sent as a bulk transfer
*/
static void LIBUSB_CALL send_cb(struct libusb_transfer *transfer)
{
    if (do_exit == 2 | did_send == 2)
        return;
    if (transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        did_send = 2;
        libusb_free_transfer(transfer);
        return;
    }

    #ifdef DEBUG_SEND_CB
    printf("Length: %d   ", transfer->actual_length);
    printf("Send callback[");
    for (int i = 0; i < transfer->actual_length; ++i)
    {
        //printf("%.2X ", recvbuf[i]);
        putchar(transfer->buffer[i]);
        printf(" ");

    }
    fflush(stdout);
    printf("]\n");
    #endif

    int r = libusb_submit_transfer(send_bulk_transfer);
    if (r < 0)
    {
        //fprintf(stderr, "LIBUSB_SUBMIT_ERROR ERROR: %s   :   send_cb 2\n", libusb_error_name(r));
        did_send = 0;
    }
    else
        did_send = 2;
}

/**
 * This is a helper function to abstract sending data in an async manner
 * This sends the bulk transfer in a send format and handles the send callback
 * 
 * @param endpoint This is the output endpoint of the device, because it is received by libusb (ex. EP_DATA_OUT, `lsusb -vv`-> bEndpointAddress     0x02  EP 2 OUT)
 * @param msg This is the message that will be sent to the device
 * @param ctx The context the control transfer should be handled within
*/
int sendData(uint8_t endpoint, unsigned char * msg, libusb_context * ctx = NULL)
{
    int r;
    unsigned char sendbuf[SEND_BUFFER_SIZE];
    memcpy(sendbuf, msg, sizeof(sendbuf));
    if ((r = strlen((const char *)sendbuf)) <= 0)
    {
        return r;
    }
    else
    {
        int len = r;
        #ifdef DEBUG_TO_CB
        printf("In sendData loop\n");
        #endif
        
        send_bulk_transfer = libusb_alloc_transfer(0);
        if (!send_bulk_transfer)
        {
            fprintf(stderr, "LIBUSB_ALLOC_TRANSFER ERROR\n");
            return -1;
        }

        #ifdef DEBUG_TO_CB
        printf("Length: %d   Msg Size: %ld \n", len, sizeof(msg));
        printf("Send Endpoint: %d\n",endpoint);
        #endif

        //r = libusb_bulk_transfer(devh, EP_DATA_OUT, sendbuf, len, &transferred, 200);
        libusb_fill_bulk_transfer(send_bulk_transfer, devh, endpoint, sendbuf,
                                len, send_cb, NULL, 200);

        r = libusb_submit_transfer(send_bulk_transfer);
        if (r < 0)
        {
            // Really only fails when you unplug, ie hotplug picks up slack
            //fprintf(stderr, "LIBUSB_SUBMIT_TRANSFER ERROR: %s   :   sendData 1\n", libusb_error_name(r));
            libusb_free_transfer(send_bulk_transfer);
            return -1;
        }
        
        #ifdef DEBUG_TO_CB
        printf("read[%d]\n", len);
        printf("Before handle = do_exit[%d] ; did_send[%d]\n", do_exit, did_send);
        #endif

        while (!did_send)
        {
            // Receive data callback
            struct timeval tv = {0L, 0L};
            int r = libusb_handle_events_timeout(ctx, &tv);
            if (r < 0)
            {
                libusb_free_transfer(send_bulk_transfer);
                return -1;
            }
        }
        #ifdef DEBUG_TO_CB
        printf("sendData : Exited the loop\n");
        #endif
    }
    return 0; // Set up so it must recieve before ever sending, 1 -> didnt fail, though didnt send either
}

/**
 * This is the hotplug attach callback which handles initializing the device and sending and recieving
 * 
 * @param ctx The context the control transfer should be handled within
 * @param dev The device that was connected in the event
 * @param event The hotplug event that happened (In this case it would be LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
 * @param user_data Any user data that you would like to access within the callback (ComData)
*/
static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
	struct libusb_device_descriptor desc;
	int rc;

	(void)ctx;
	(void)dev;
	(void)event;
	(void)user_data;

    uint8_t tempNumIfaces;
    uint8_t tempEP_IN;
    uint8_t tempEP_OUT;
    DEVICE devType;
    

    #ifdef DEBUG_TO_TERMINAL
    printf("Init gen_ctx\n");
    #endif

    rc = libusb_init(&gen_ctx);
    if (rc < 0)
    {
        fprintf(stderr, "failed to initialise libusb\n");
        exit(1);
    }


	rc = libusb_get_device_descriptor(dev, &desc);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error getting device descriptor\n");
	}

    #ifdef DEBUG_TO_TERMINAL
	printf ("Device attached: %04x:%04x\n", desc.idVendor, desc.idProduct);
    #endif

	if (devh) {
		libusb_close (devh);
		devh = NULL;
	}

    devh = libusb_open_device_with_vid_pid(gen_ctx, desc.idVendor, desc.idProduct);
    if (devh == NULL)
    {
        fprintf(stderr, "Could not find/open device\n");
        return -1;
    }

    if (desc.idVendor == ARDUINO::vendorID && desc.idProduct == ARDUINO::productID)
    {
        tempNumIfaces = ARDUINO::NUM_INTERFACES;
        tempEP_IN     = ARDUINO::EP_DATA_IN;
        tempEP_OUT    = ARDUINO::EP_DATA_OUT;
        devType       = DEVICE::ARDUINO_type;
    }
    else if (desc.idVendor == CH34x::vendorID && desc.idProduct == CH34x::productID)
    {
        tempNumIfaces = CH34x::NUM_INTERFACES;
        tempEP_IN     = CH34x::EP_DATA_IN;
        tempEP_OUT    = CH34x::EP_DATA_OUT;
        devType       = DEVICE::CH34x_type;
    }
    else
    {
        fprintf(stderr, "Could not find/open device\n");
        return -1;
    }

    for (int iface = 0; iface < tempNumIfaces; iface++)
    {
        if (libusb_kernel_driver_active(devh, iface)) {
            libusb_detach_kernel_driver(devh, iface);
        }
        rc = libusb_claim_interface(devh, iface);
        if (rc < 0)
        {
            #ifdef DEBUG_TO_TERMINAL
            fprintf(stderr, "USB_CLAIM_INTERFACE ERROR: %s\n", libusb_error_name(rc));
            #endif
            while (rc != 0)
            {
                std::this_thread::sleep_for(1s);
                #ifdef DEBUG_TO_TERMINAL
                printf("Trying to claim IFACE again\n");
                #endif
                rc = libusb_claim_interface(devh, iface);
                fprintf(stderr, "USB_CLAIM_INTERFACE ERROR: %d : %s\n", rc,  libusb_error_name(rc));
                if (rc == -4)
                {
                    return -1;
                }
            }
        }
    }

    #ifdef DEBUG_TO_TERMINAL
    printf("claimed interface\n");
    #endif

    if (devType == DEVICE::CH34x_type)
        rc = safe_init_ch34x(CH34x::DEFAULT_BAUD_RATE, gen_ctx); // If we return < 0 here a control transfer failed
    else if (devType == DEVICE::ARDUINO_type)
        rc = safe_init_ATmega32u4(ARDUINO::DEFAULT_BAUD_RATE, gen_ctx); // If we return < 0 here a control transfer failed
    else
    {
        fprintf(stderr, "Could not find/open device\n");
        return -1;
    }

    if (rc < 0)
    {
        printf("Released\n");
        for (int iface = 0; iface < tempNumIfaces; iface++)
        {
            libusb_release_interface(devh, iface);
        }
        return -1;
    }

    #ifdef DEBUG_TO_TERMINAL
    printf("initialized\n");
    #endif

    std::this_thread::sleep_for(1s);
    int iterate = 0;
    while (!do_exit)
    {
        iterate++;

        // rc = sendData(tempEP_OUT, SEND_TEXT, gen_ctx);
        // if (rc < 0)
        // {
        //     #ifdef DEBUG_TO_TERMINAL
        //     printf("Send Data failed.\n");
        //     #endif
        //     do_exit = 2;
        //     return 0;
        // }
        did_recv = 0;
        rc = recvData(tempEP_IN, gen_ctx, (ComData*)user_data);
        if (rc < 0)
        {
            #ifdef DEBUG_TO_TERMINAL
            printf("Recv Data failed.\n");
            #endif
            do_exit = 2;
            return 0;
        }
        did_send = 0;

        // std::this_thread::sleep_for(26ms); // Pretty much just ABCDE
        std::this_thread::sleep_for(20ms);

        
        #ifdef DEBUG_TO_TERMINAL
        printf("iterate [%d] ",iterate%2);
        #endif

        if (iterate %2 == 0)
        {
            unsigned char temp[] = "Hi does this";
            memcpy(SEND_TEXT,temp, sizeof(temp));
        }
        if (iterate %2 == 1)
        {
            unsigned char temp[] = "Maybe this!!";
            memcpy(SEND_TEXT,temp, sizeof(temp));
        }

        #ifdef DEBUG_TO_TERMINAL
        printf("--- Done iteration [%d] ---\n", iterate);
        #endif
    }

    #ifdef DEBUG_TO_TERMINAL
	printf("Output from rc: %d\n", rc);
    #endif

	return 0;
}

/**
 * This is the hotplug detach callback which handles deinitializing the device and closing the general context
 * 
 * @param ctx The context the control transfer should be handled within
 * @param dev The device that was connected in the event
 * @param event The hotplug event that happened (In this case it would be LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
 * @param user_data Any user data that you would like to access within the callback (ComData)
*/
static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
	(void)ctx;
	(void)dev;
	(void)event;
	(void)user_data;

    #ifdef DEBUG_TO_TERMINAL
	printf ("Device detached\n");
    #endif

    do_exit = 0;
    ctrl_exit = 0;
    did_recv = 0;
    did_send = 0;

	if (devh) {
		libusb_close (devh);
		devh = NULL;
	}
    libusb_exit (gen_ctx);

    #ifdef DEBUG_TO_TERMINAL
    printf("Detach Finished\n");
    #endif

	return 0;
}

int runHotplug(ComData* data, uint16_t vendorID, uint16_t productID)
{
    libusb_device ** devList;
    libusb_device * singleDev;
    ssize_t cnt;
    int i = 0;

	libusb_hotplug_callback_handle hp[2];
	int rc;
    int classID   =  LIBUSB_HOTPLUG_MATCH_ANY;

	rc = libusb_init(&hp_ctx);
	if (rc < 0)
	{
		printf("failed to initialise libusb: %s\n", libusb_error_name(rc));
		return EXIT_FAILURE;
	}

	if (!libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG)) {
		printf ("Hotplug capabilities are not supported on this platform\n");
		libusb_exit (hp_ctx);
		return EXIT_FAILURE;
	}

	rc = libusb_hotplug_register_callback (hp_ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, LIBUSB_HOTPLUG_NO_FLAGS, vendorID,
		productID, classID, hotplug_callback, data, &hp[0]);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 0 : %s\n", libusb_error_name(rc));
		libusb_exit (hp_ctx);
		return EXIT_FAILURE;
	}

	rc = libusb_hotplug_register_callback (hp_ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_NO_FLAGS, vendorID,
		productID,classID, hotplug_callback_detach, NULL, &hp[1]);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 1 : %s\n", libusb_error_name(rc));
		libusb_exit (hp_ctx);
		return EXIT_FAILURE;
	}

    // Check the current list of devices then run a hotplug arrived call
    // hotplug_callback(hp_ctx, libusb_device *dev, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, data);
    printf ("Actual Info: %04x:%04x\n", vendorID, productID);
    cnt = libusb_get_device_list(NULL, &devList);
	if (cnt != 0){
        while ((singleDev = devList[i++]) != NULL) {
            struct libusb_device_descriptor desc;

		    int r = libusb_get_device_descriptor(singleDev, &desc);
            if (r < 0) 
            {
                // If it is unable to get the device descriptor we dont care, since it cant be the VID/PID we need
                break;
		    }

            printf ("Debug: %04x:%04x\n", desc.idVendor, desc.idProduct);
            
            if (desc.idVendor == vendorID && desc.idProduct == productID)
            {
                printf ("Ran hotplug_cb\n");
                hotplug_callback(hp_ctx, singleDev, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, data);
                break;
            }
        }
	}
    
	while (!hp_done) {
		rc = libusb_handle_events (hp_ctx);
		if (rc < 0)
			printf("libusb_handle_events() failed: %s\n", libusb_error_name(rc));
	}

	if (devh) {
		libusb_close (devh);
	}

	libusb_exit (hp_ctx);

	return EXIT_SUCCESS;
}
