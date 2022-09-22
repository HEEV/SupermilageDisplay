#include "MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>

#include "Profiler.h"

//==============================================================================
MainComponent::MainComponent() :
    _burn("Burn", Colours::red),
    _speed("Vehicle MPH", 0.0f, 30.0f, Colour(253, 185, 19), 6),
    _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)),
    _temp(0, 200, 20),
    _volt(0, 12, 1),
    _counter(2.22475f, 4)
{
    FUNCTION_PROFILE();
    addAndMakeVisible(_burn);
    addAndMakeVisible(_speed);
    addAndMakeVisible(_wind);
    addAndMakeVisible(_timer);
    addAndMakeVisible(_counter);
    addAndMakeVisible(_volt);
    addAndMakeVisible(_temp);
    _speed.addLapCounter(&_counter);

    addMouseListener(&_mouse, true);
    
    setSize(getParentWidth(), getParentHeight());

    _dataStream.open("data.txt", std::ios::out | std::ios::app);
    if (_dataStream.is_open())
    {
        _dataStream << "Begin Race\n";
        _dataStream << "Time, Speed, Wind Speed\n";
    }
}

MainComponent::~MainComponent()
{
    if(_dataStream.is_open())
        _dataStream.close();
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

    //_temp.setBounds(row.removeFromLeft(row.getWidth() / 2));
    //row.removeFromLeft(marginSmall);
    //_volt.setBounds(row.removeFromLeft(row.getWidth() - marginSmall));

    auto topHalf = bounds.removeFromTop(bounds.getHeight() / 2);

    _timer.setBounds(topHalf.removeFromTop(topHalf.getHeight() / 2));
    
    _counter.setBounds(bounds);
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}
