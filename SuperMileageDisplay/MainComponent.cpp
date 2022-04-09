#include "MainComponent.h"
#include <string>

#include "Profiler.h"

//==============================================================================
MainComponent::MainComponent() :
    _burn("Burn", Colours::red),
    _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19)),
    _wind("Wind MPH", 0.0f, 50.0f, Colour(253, 185, 19), 10),
    _engTemp("Eng. Temp.", 100, 200, Colour(253, 185, 19), 4),
    _volts("Bat. Volt.", 0, 12, Colour(253, 185, 19), 4),
    _counter(10.0, 3),
    _ComManager(ComCenter((Delegate*)this))
{
    FUNCTION_PROFILE();
    addAndMakeVisible(_burn);
    addAndMakeVisible(_speed);
    addAndMakeVisible(_wind);
    addAndMakeVisible(_engTemp);
    addAndMakeVisible(_volts);
    addAndMakeVisible(_timer);
    addAndMakeVisible(_counter);

    setSize(getParentWidth(), getParentHeight());

    _ComManager.initalize();
}

MainComponent::~MainComponent()
{
    FUNCTION_PROFILE();
}

//==============================================================================
//Function exicuts every frame
void MainComponent::paint (juce::Graphics& g)
{

}

void MainComponent::resized()
{
    FUNCTION_PROFILE();
    constexpr int margin = 50;
    constexpr int marginSmall = 10;
    constexpr int mainMeterSize = 250;
    constexpr int rowSize = 200;

    auto bounds = getLocalBounds();
    bounds.removeFromTop(margin);

    auto row = bounds.removeFromTop(rowSize);
    row.removeFromLeft(margin);
    row.removeFromRight(margin);

    _speed.setBounds(row.removeFromLeft(mainMeterSize));
    _wind.setBounds(row.removeFromRight(mainMeterSize));
    _burn.setBounds(row.removeFromTop(row.getHeight() / 4));

    row.removeFromLeft(marginSmall);

    _engTemp.setBounds(row.removeFromLeft(row.getWidth() / 2));
    row.removeFromLeft(marginSmall);
    _volts.setBounds(row.removeFromLeft(row.getWidth() - marginSmall));
    _timer.setBounds(bounds.removeFromTop(bounds.getHeight() / 2));
    
    _counter.setBounds(bounds);
}

void MainComponent::updateHandler(std::string topic, SensorData msg) 
{
    FUNCTION_PROFILE();
    switch (msg.id) {
    case(SensorType::Speed):
        _speed.setData(msg.data);
        break;
    case(SensorType::Wind):
        _wind.setData(msg.data);
        break;
    case(SensorType::EngineTemperature):
        _engTemp.setData(msg.data);
        break;
    case(SensorType::BatterVoltage):
        _volts.setData(msg.data);
        break;
    case(SensorType::Burn):

        break;
    }
}
