#pragma once
#include <JuceHeader.h>
#include "Display/RaceTimer.h"
#include "Display/LapCounter.h"
#include "Display/MapComponent.h"

class MainComponent;

class ClickLayer : public Component 
{
    public:
        ClickLayer(RaceTimer* timer, LapCounter* lap, MapComponent* map);

        void mouseDown(const MouseEvent& event) override;
        void mouseDoubleClick(const MouseEvent& e) override;
    private:
        RaceTimer* _timer;
        LapCounter* _lapCount;
        MapComponent* _mapComp;
};