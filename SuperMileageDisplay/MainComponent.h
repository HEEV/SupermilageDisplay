#pragma once

#include <JuceHeader.h>
#include <fstream>

#include "Speedometer.h"
#include "ColorLight.h"
#include "RaceTimer.h"
#include "LapCounter.h"
#include "VerticleGauge.h"
#include "MapComponent.h"
#include "Tiltmeter.h"

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
class MainComponent  : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void update() override;
    
    //DataValues Storage;

private:
    Speedometer _speed;
    Speedometer _wind;
    MapComponent _map;
    Tiltmeter _tilt;
    RaceTimer _timer;
    LapCounter _counter;
    VerticleGauge _engTemp;
    VerticleGauge _volt;


    class MouseEvents : public MouseListener
    {
    public:
        void mouseDoubleClick(const MouseEvent& e) override;
    } _mouse;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

};
