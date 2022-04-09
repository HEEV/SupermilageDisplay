#pragma once

#include <JuceHeader.h>
#include <ComCenter.h>
#include <Delegate.h>

#include "Speedometer.h"
#include "ColorLight.h"
#include "RaceTimer.h"
#include "LapCounter.h"

enum SensorType {
    Speed = '1',
    Wind = '2',
    EngineTemperature = '3',
    BatterVoltage = '4',
    Burn = '5'
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, Delegate
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateHandler(std::string topic, SensorData msg) override;

    
    //DataValues Storage;

private:
    Speedometer _speed;
    Speedometer _wind;
    Speedometer _engTemp;
    Speedometer _volts;

    ColorLight _burn;

    RaceTimer _timer;

    LapCounter _counter;

    ComCenter _ComManager;
    std::stringstream _DebugStream;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
