#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : _speed("Vehicle MPH", 0.0f, 40.0f, juce::Colour(255, 255, 0))
{
    addAndMakeVisible(_speed);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
//Function exicuts every frame
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    _speed.setBounds(10, 10, 100, 100);
}
