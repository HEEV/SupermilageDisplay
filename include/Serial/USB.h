#pragma once
#ifdef WIN32
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif
#include <functional>
#include <map>
#include <thread>

class USB
{
public:
    USB(uint8_t endpointIn,
        uint8_t endpointOut,
        uint8_t endpointCtrlOut,
        uint8_t dtr,
        uint8_t rts,
        uint8_t numInterfaces,
        uint16_t baudrate,
        uint16_t vendorID,
        uint16_t prodID,
        uint16_t sendBufSize);
    virtual ~USB();

    libusb_device_handle* hotplugAttachCB();
    int hotplugDetachCB();
    void controlCB(libusb_transfer* transfer);
    void dataRecieveCB(libusb_transfer* transfer);
    void dataSendCB(libusb_transfer* transfer);
    void sendMsg(std::byte* msg);

protected:
    void m_startSendRecv();

private:
    uint8_t _endpointIn;
    uint8_t _endpointOut;
    uint8_t _endpointCtrlOut;
    uint8_t _dtr;
    uint8_t _rts;
    uint8_t _numInterfaces;
    uint16_t _baudRate;
    uint16_t _vendorID;
    uint16_t _productID;
    uint16_t _sendBufferSize;

    libusb_device_handle* _devh;
    libusb_context* _hotPlugcx;
    libusb_context* _gencx;
    libusb_transfer* _recvBulkTransfer;
    libusb_transfer* _sendBulkTransfer;

    uint8_t _doExit;
    uint8_t _ctrlExit;
    uint8_t _didRecv;
    uint8_t _didSend;
    uint8_t _hpDone;
    uint8_t recvBuffer[1024];

    std::thread _worker;

    static std::map<libusb_context*, USB*> _hotplugThis;
    static std::map<libusb_device_handle*, USB*> _genThis;

    //  void _rcvMsg();
    //  void _ctrlMsg(libusb_transfer* cntrlTransfer, uint8_t request, uint16_t val, uint16_t index, uint16_t len = 0, std::byte* data = nullptr);

    friend int LIBUSB_CALL libusbHotplugCB(libusb_context* ctx, libusb_device* dev, libusb_hotplug_event event, void* userData);
    friend void LIBUSB_CALL libUSBControlCB(libusb_transfer* transfer);
    friend void LIBUSB_CALL libUSBRecieveCB(libusb_transfer* transfer);
    friend void LIBUSB_CALL libUSBSendCB(libusb_transfer* transfer);

};
