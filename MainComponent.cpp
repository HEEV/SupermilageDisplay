#include "MainComponent.h"
#include "../Builds/VisualStudio2019/Speedometer.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    setFramesPerSecond(60);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::royalblue);

    juce::Point<int> speedP(getWidth() / 4, getHeight() / 4);
    Speedometer speed(speedP);
    speed.setDataRange(0, 30);
    speed.updateDataValue(rotate);
    rotate++;
    speed.draw(g);

    juce::Font mainComponentFont("Consolas", 20.0f, juce::Font::bold);
    g.setFont(mainComponentFont);
    g.setColour(juce::Colours::white);
    g.drawText(currentSizeAsString, getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    currentSizeAsString = juce::String(getWidth()) + " x " + juce::String(getHeight());
}

void MainComponent::update() {

}
