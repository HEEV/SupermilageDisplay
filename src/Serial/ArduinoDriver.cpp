// Compile like g++ ArduinoDriver.cpp -o ArduinoDriver.o -lusb-1.0 [Outside of full project]

// Runs transfers in async mode, and has hotplug support
#include "Serial/ArduinoDriver.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <iostream>

#include <libusb-1.0/libusb.h>

//TODO: Remove include
#include <CommunicationManager.h>
#include <Packets.h>
CommunicationManager* man;
int wheelID, windID, engID, tiltID, gpsID;

using namespace std::chrono_literals;

// #define DEBUG_TO_TERMINAL
// #define DEBUG_TO_CB
#define DEBUG_CTRL_CB
#define DEBUG_SEND_CB
#define DEBUG_RECV_CB

#define EP_DATA_IN (0x2 | LIBUSB_ENDPOINT_IN)
#define EP_DATA_OUT (0x2 | LIBUSB_ENDPOINT_OUT)
#define CTRL_IN (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)

#define DEFAULT_BAUD_RATE 9600
#define NUM_INTERFACES 1

static struct libusb_device_handle *devh = NULL;
static struct libusb_context * hp_ctx = NULL;
static struct libusb_context * gen_ctx = NULL;
static struct libusb_transfer *recv_bulk_transfer = NULL;
static struct libusb_transfer *send_bulk_transfer = NULL;

// Vendor defined -> see ch34x.c driver
enum
{
    VENDOR_READ				= 0x95, // Unused
    VENDOR_WRITE			= 0x9A,
    VENDOR_SERIAL_INIT		= 0xA1,
    VENDOR_MODEM_OUT		= 0xA4, // Handshake
    VENDOR_VERSION			= 0x5F  // Unused
};

// Ch34x (ie fake arduino's)
uint16_t vendorID = 0x1A86;
uint16_t productID = 0x7523;

// Adafruit Feather 32u4
// uint16_t vendorID = 0x239A;
// uint16_t productID = 0x800C;

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

uint8_t dtr = 0;
uint8_t rts = 0;

#ifdef DEBUG_TO_TERMINAL
int runSetup = 0;
#endif

uint8_t do_exit = 0;
uint8_t ctrl_exit = 0;
uint8_t did_recv = 0;
uint8_t did_send = 0;
uint8_t hp_done = 0;

uint8_t recvbuf[1024];
int SEND_BUFFER_SIZE = 32;

int iterations = 0;

unsigned char SEND_TEXT[] = "HELLO WORLD!";

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
    unsigned char test[transfer->length];
    memcpy(test, transfer->buffer, transfer->length);
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

int ctrl_transfer (libusb_transfer * transfer, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, libusb_context * ctx = NULL)
{
    unsigned char buf[LIBUSB_CONTROL_SETUP_SIZE + 1];
    transfer = libusb_alloc_transfer(0);

	libusb_fill_control_setup(buf, CTRL_OUT, bRequest, wValue, wIndex, 0);

    #ifdef DEBUG_TO_TERMINAL
    runSetup++;
	printf("[%d] Buffer Before Sent:", runSetup);
    for (int i = 0; i < sizeof(buf); ++i)
    {
        printf("%.2X ", buf[i]);
    }
    printf("\n");
    #endif
    
    libusb_fill_control_transfer(transfer, devh, buf, ctrl_setup_cb, NULL, 1000);

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

int init_baudrate(struct libusb_transfer * transfer, int baudRate, libusb_context * ctx = NULL)
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
	        r = ctrl_transfer(transfer, VENDOR_WRITE, 0x1312, baudEncoding[i * 3 + 1], ctx);
            if (r < 0)
            {
                fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x1312\n");
                return r;
            }
            ctrl_exit = 0;

            //r = libusb_control_transfer(devh, CTRL_OUT, 0x9a, 0x0f2c, baudEncoding[i * 3 + 2], NULL, 0, 1000);
	        r = ctrl_transfer(transfer, VENDOR_WRITE, 0x0f2c, baudEncoding[i * 3 + 2], ctx);
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

int safe_init_ch34x(int baudRate, libusb_context * ctx = NULL)
{
    int r;
    unsigned char buf[LIBUSB_CONTROL_SETUP_SIZE + 1];
	struct libusb_transfer *transfer;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0xa1, 0, 0, NULL, 0, 1000);
    r = ctrl_transfer(transfer, VENDOR_SERIAL_INIT, 0, 0, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_SERIAL_INIT\n");
        return r;
    }

    ctrl_exit = 0;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0x9a, 0x2518, 0x0050, NULL, 0, 1000);
    r = ctrl_transfer(transfer, VENDOR_WRITE, 0x2518, 0x0050, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_WRITE,0x2518\n");
        return r;
    }

    ctrl_exit = 0;

    //r = libusb_control_transfer(devh, CTRL_OUT, 0xa1, 0x501f, 0xd90a, NULL, 0, 1000);
	r = ctrl_transfer(transfer, VENDOR_SERIAL_INIT, 0x501f, 0xd90a, ctx);
    if (r < 0)
    {
        fprintf(stderr, "Failed control transfer VENDOR_SERIAL_INIT,0x501f\n");
        return r;
    }

    ctrl_exit = 0;
    
    // --- BAUDRATE ---
    r = init_baudrate(transfer, baudRate, ctx);
    if (r < 0)
        return r;
    // Does ctrl_exit = 0; inside init_baudrate()

    // --- HANDSHAKE ---
    //if (libusb_control_transfer(devh, CTRL_OUT, 0xa4, ~((dtr ? 1 << 5 : 0) | (rts ? 1 << 6 : 0)), 0, NULL, 0, 1000) < 0)
	r = ctrl_transfer(transfer, VENDOR_MODEM_OUT, ~((dtr ? 1 << 5 : 0) | (rts ? 1 << 6 : 0)), 0, ctx);
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

static void LIBUSB_CALL recv_cb(struct libusb_transfer *transfer)
{
    if (do_exit == 2 | did_recv == 2)
        return;
    if (transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        did_recv = 2;
        libusb_free_transfer(transfer);
        return;
    }


    int r = libusb_submit_transfer(recv_bulk_transfer);
    if (r < 0)
    {
        //fprintf(stderr, "LIBUSB_TRANSFER_ERROR ERROR: %s   :   recv_cb 2\n", libusb_error_name(r));
        did_recv = 0;
    }
    else
    {
        did_recv = 2;

        // TODO: Remove Following Code
        struct __attribute__ ((packed)) Data
        {
            int16_t sensor;
            float data;
            int64_t time;
        };
        Data* data = &((Data*)recvbuf)[0];
        WheelData wd;
        WindSpeed ws;
        EngineTemp et;
        CarTilt ct;
        printf("%d : ", data->sensor);
        printf("%f : ", data->data);
        printf("%ld\n", data->time);
        switch(data->sensor)
        {
        case 0:
            wd.velocity(data->data);
            wd.distTravelled(data->time * data->data);
            wd.head().timeOcc(data->time);
            man->writeData(wheelID, &wd);
            break;
        case 1:
            ws.headSpeed(data->data);
            ws.head().timeOcc(data->time);
            man->writeData(windID, &ws);
            break;
        case 2:
            et.temp(data->data);
            et.head().timeOcc(data->time);
            man->writeData(engID, &et);
            break;
        case 3:
            ct.angle(data->data);
            ct.head().timeOcc(data->time);
            man->writeData(tiltID, &ct);
            break;
        }

        // ------------------------

        #ifdef DEBUG_RECV_CB
        printf("Data callback[");
        for (int i = 0; i < transfer->actual_length; ++i)
        {
            printf("%.2X ", recvbuf[i]);
            // putchar(recvbuf[i]);
            printf(" ");

        }
        // printf("  -Test float: %f", ((float*)recvbuf)[0]);
        // printf("  -Test float: %f", ((float*)recvbuf)[1]);
        fflush(stdout);
        printf("]\n");
        #endif
    }
}

int recvData(libusb_context * ctx = NULL)
{
    int r;
    #ifdef DEBUG_TO_CB
    printf("In recvData loop\n");
    #endif
    recv_bulk_transfer = libusb_alloc_transfer(0);
    if (!recv_bulk_transfer)
    {
        fprintf(stderr, "LIBUSB_ALLOC_TRANSFER ERROR\n");
        return -1;
    }

    libusb_fill_bulk_transfer(recv_bulk_transfer, devh, EP_DATA_IN, recvbuf,
                            sizeof(recvbuf), recv_cb, NULL, 0);

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

int sendData(unsigned char * msg, libusb_context * ctx = NULL)
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
        #endif

        //r = libusb_bulk_transfer(devh, EP_DATA_OUT, sendbuf, len, &transferred, 200);
        libusb_fill_bulk_transfer(send_bulk_transfer, devh, EP_DATA_OUT, sendbuf,
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

static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
	struct libusb_device_descriptor desc;
	int rc;

	(void)ctx;
	(void)dev;
	(void)event;
	(void)user_data;

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
        libusb_close(devh);
        libusb_exit(gen_ctx);
        return -1;
    }

    for (int iface = 0; iface < NUM_INTERFACES; iface++)
    {
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
                fprintf(stderr, "USB_CLAIM_INTERFACE ERROR: %s\n", libusb_error_name(rc));
            }
        }
    }

    #ifdef DEBUG_TO_TERMINAL
    printf("claimed interface\n");
    #endif

    rc = safe_init_ch34x(DEFAULT_BAUD_RATE, gen_ctx); // If we return < 0 here a control transfer failed
    if (rc < 0)
    {
        libusb_release_interface(devh, 0);
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

        rc = sendData(SEND_TEXT, gen_ctx);
        if (rc < 0)
        {
            #ifdef DEBUG_TO_TERMINAL
            printf("Send Data failed.\n");
            #endif

            for (int iface = 0; iface < NUM_INTERFACES; iface++)
            {
                libusb_release_interface(devh, iface);
            }
            do_exit = 2;
            return 0;
        }
        did_recv = 0;
        rc = recvData(gen_ctx);
        if (rc < 0)
        {
            #ifdef DEBUG_TO_TERMINAL
            printf("Recv Data failed.\n");
            #endif

            for (int iface = 0; iface < NUM_INTERFACES; iface++)
            {
                libusb_release_interface(devh, iface);
            }
            do_exit = 2;
            return 0;
        }
        did_send = 0;

        std::this_thread::sleep_for(100ms);

        
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

int runHotplug()
{
    // TODO: REMOVE CODE
    man = new CommunicationManager("163.11.237.241:5001");
    REGISTER_TYPE_TO_MANAGER(WheelData, "vel", (*man));
    REGISTER_TYPE_TO_MANAGER(WindSpeed, "wind", (*man));
    REGISTER_TYPE_TO_MANAGER(EngineTemp, "enTemp", (*man));
    REGISTER_TYPE_TO_MANAGER(CarTilt, "tilt", (*man));
    REGISTER_TYPE_TO_MANAGER(GPSPosition, "gps", (*man));

    wheelID = man->addDataWriter("vel");
    windID = man->addDataWriter("wind");
    engID = man->addDataWriter("enTemp");
    tiltID = man->addDataWriter("tilt");
    gpsID = man->addDataWriter("gps");
    // ----------------------

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

	rc = libusb_hotplug_register_callback (hp_ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, 0, vendorID,
		productID, classID, hotplug_callback, NULL, &hp[0]);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 0 : %s\n", libusb_error_name(rc));
		libusb_exit (hp_ctx);
		return EXIT_FAILURE;
	}

	rc = libusb_hotplug_register_callback (hp_ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, vendorID,
		productID,classID, hotplug_callback_detach, NULL, &hp[1]);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 1 : %s\n", libusb_error_name(rc));
		libusb_exit (hp_ctx);
		return EXIT_FAILURE;
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
