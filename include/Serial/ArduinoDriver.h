#pragma once

class CommunicationManager;

struct ComData
{
    CommunicationManager* man;
    int wheelID;
    int windID;
    int engID;
    int tiltID;
    int gpsID;
    int batID;
};

/**
 * Creates a libusb connection with the given device and connects it to FAST-DDS, handles hotplug, initialization of devices and sending/recieving
 * 
 * @param data A pointer to ComData, a struct which has the communication manager, as well as sensor id's
 * @param vendorID The vendor id of the device you want to connect libusb to (`lsusb`-> ID <VendorID>:<ProductID> <Name of device>)
 * @param productID The product id of the device you want to connect libusb to (`lsusb`-> ID <VendorID>:<ProductID> <Name of device>)
*/
int runHotplug(ComData* data, unsigned short vendorID, unsigned short productID);
