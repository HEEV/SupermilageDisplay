#pragma once

#include <JuceHeader.h>
#include "DataValues.h"

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
    
    DataValues Storage;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::String currentSizeAsString;

    juce::Point<int> speedP1;
    juce::Point<int> speedP2;
    juce::Point<int> speedP3;
    juce::Point<int> speedP4;
    juce::Point<int> speedP5;
    juce::Point<int> speedP6;
    juce::Point<int> speedP7;
    juce::Point<int> speedP8;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

    float rotate = 0;
};
