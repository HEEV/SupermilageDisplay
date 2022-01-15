#include "MainComponent.h"
#include "Speedometer.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 260);
    setFramesPerSecond(60);
    speedP1.setXY(100, 80);
    speedP2.setXY(300, 80);
    speedP3.setXY(500, 80);
    speedP4.setXY(700, 80);
    speedP5.setXY(100, 210);
    speedP6.setXY(300, 210);
    speedP7.setXY(500, 210);
    speedP8.setXY(700, 210);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::royalblue);

    Speedometer speed1(speedP1, 0, 30, "Speedometer");
    speed1.setData(rotate);
    speed1.draw(g);
    Speedometer speed2(speedP2, 0, 30, "Speedometer");
    speed2.setData(rotate);
    speed2.draw(g);
    Speedometer speed3(speedP3, 0, 30, "Speedometer");
    speed3.setData(rotate);
    speed3.draw(g);
    Speedometer speed4(speedP4, 0, 30, "Speedometer");
    speed4.setData(rotate);
    speed4.draw(g);
    Speedometer speed5(speedP5, 0, 30, "Speedometer");
    speed5.setData(rotate);
    speed5.draw(g);
    Speedometer speed6(speedP6, 0, 30, "Speedometer");
    speed6.setData(rotate);
    speed6.draw(g);
    Speedometer speed7(speedP7, 0, 30, "Speedometer");
    speed7.setData(rotate);
    speed7.draw(g);
    Speedometer speed8(speedP8, 0, 30, "Speedometer");
    speed8.setData(rotate);
    speed8.draw(g);

    if(rotate < 30) rotate += 0.5;

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
