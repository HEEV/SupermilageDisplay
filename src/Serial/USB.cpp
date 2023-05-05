#include "Serial/USB.h"
#include <algorithm>
#include <fmt/format-inl.h>
#include <thread>
#include <chrono>

#ifndef __linux
#define __linux 0
#endif

using namespace std::chrono_literals;

std::map<libusb_context*, USB*> USB::_hotplugThis;
std::map<libusb_device_handle*, USB*> USB::_genThis;

int LIBUSB_CALL libusbHotplugCB(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *userData)
{
    USB* th = USB::_hotplugThis.at(ctx);
    libusb_device_handle* handle;
    int ret = 0;
    switch(event)
    {
    case LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED:
        handle = th->hotplugAttachCB();
        USB::_genThis.insert(std::make_pair(handle, th));
        ret = ((handle == nullptr) ? 1 : 0);
        break;
    case LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT:
        ret = th->hotplugDetachCB();
        std::erase_if(USB::_genThis, [th](const auto& c){ return c.second == th; });
        break;
    }

    return ret;
}

void LIBUSB_CALL libUSBControlCB(libusb_transfer* transfer)
{
    USB::_genThis.at(transfer->dev_handle)->controlCB(transfer);
}

void LIBUSB_CALL libUSBRecieveCB(libusb_transfer* transfer)
{
    USB::_genThis.at(transfer->dev_handle)->dataRecieveCB(transfer);
}

void LIBUSB_CALL libUSBSendCB(libusb_transfer* transfer)
{
    USB::_genThis.at(transfer->dev_handle)->dataSendCB(transfer);
}

USB::USB(uint8_t endpointIn,
        uint8_t endpointOut,
        uint8_t endpointCtrlOut,
        uint8_t dtr, uint8_t rts,
        uint8_t numInterfaces,
        uint16_t baudrate,
        uint16_t vendorID,
        uint16_t prodID,
        uint16_t sendBufSize) :
        _endpointIn(endpointIn),
        _endpointOut(endpointOut),
        _endpointCtrlOut(endpointCtrlOut),
        _dtr(dtr), _rts(rts),
        _numInterfaces(numInterfaces),
        _baudRate(baudrate),
        _vendorID(vendorID),
        _productID(prodID),
        _sendBufferSize(sendBufSize),
        _hpDone(false), _doExit(false)
{
    libusb_hotplug_callback_handle hp;
    constexpr int classID = LIBUSB_HOTPLUG_MATCH_ANY;

    int rc = libusb_init(&_hotPlugcx);
    if(rc < 0)
    {
        throw fmt::system_error(rc, "Failed to init USB {}:{}", _vendorID, _productID);
    }

    rc = libusb_hotplug_register_callback(_hotPlugcx, 
        LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
        LIBUSB_HOTPLUG_NO_FLAGS, _vendorID, _productID, classID, libusbHotplugCB, nullptr, &hp);

    rc = libusb_hotplug_register_callback(_hotPlugcx, 
        LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
        LIBUSB_HOTPLUG_NO_FLAGS, _vendorID, _productID, classID, libusbHotplugCB, nullptr, &hp);

    _worker = std::thread([this]{
        int rc;
        while(!_hpDone)
        {
            rc = libusb_handle_events(_hotPlugcx);
            if(rc < 0)
                throw fmt::system_error(rc, "libusb failed to handle events");
        }
    });
}

USB::~USB()
{
    if(_devh)
        libusb_close(_devh);
    _devh = nullptr;
    libusb_exit(_hotPlugcx);

    std::erase_if(_genThis, [this](const auto& c){ return c.second == this; });
    std::erase_if(_hotplugThis, [this](const auto& c){ return c.second == this; });
}

libusb_device_handle* USB::hotplugAttachCB()
{
    int rc = libusb_init(&_gencx);
    if(rc < 0)
    {
        throw fmt::system_error(rc, "Failed to init USB device");
    }

    if(_devh)
    {
        libusb_close(_devh);
        _devh = nullptr;
    }
    _devh = libusb_open_device_with_vid_pid(_gencx, _vendorID, _productID);

    for(int iface = 0; iface < _numInterfaces; iface++)
    {
        if(__linux && libusb_kernel_driver_active(_devh, iface))
            libusb_detach_kernel_driver(_devh, iface);
        do
        {
            rc = libusb_claim_interface(_devh, iface);
            if(rc != 0)
            {
                fmt::println("Failed to claim interace. Trying again...");
                std::this_thread::sleep_for(1s);
            }
        } while(rc != 0 && rc != -4);
    }

    return _devh;
}

int USB::hotplugDetachCB()
{
    return 0;
}

void USB::controlCB(libusb_transfer* transfer)
{

}

void USB::dataRecieveCB(libusb_transfer* transfer)
{

}

void USB::dataSendCB(libusb_transfer* transfer)
{

}

void USB::m_startSendRecv()
{
    std::this_thread::sleep_for(1s);
    while(!_doExit)
    {
        
    }
}

// void USB::sendMsg(std::byte* msg)
// {}
// void USB::_rcvMsg()
// {}
// void USB::_ctrlMsg(libusb_transfer* cntrlTransfer, uint8_t request, uint16_t val, uint16_t index, uint16_t len = 0, std::byte* data = nullptr)
// {}
