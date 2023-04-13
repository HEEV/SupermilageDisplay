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

int runHotplug(ComData* data);
