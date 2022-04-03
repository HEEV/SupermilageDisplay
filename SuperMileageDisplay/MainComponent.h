#pragma once

#include <JuceHeader.h>
#include "DataValues.h"

#include "Speedometer.h"
#include "ColorLight.h"
#include "RaceTimer.h"
#include "LapCounter.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
    //DataValues Storage;

private:
    ColorLight _burn;

    Speedometer _speed;
    Speedometer _wind;
    Speedometer _engTemp;
    Speedometer _volts;

    RaceTimer _timer;

    LapCounter _counter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
