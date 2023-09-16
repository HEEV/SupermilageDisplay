#pragma once
#include <JuceHeader.h>
#include "Display/RaceTimer.h"
#include "Display/LapCounter.h"
#include "Display/MapComponent.h"

using namespace std;

class ClickLayer : public Component 
{
    public:
        ClickLayer(function<void()> rf);

        void mouseDown(const MouseEvent& event) override;
        void mouseDoubleClick(const MouseEvent& e) override;
    private:
        function<void()> _resetFunc;
};