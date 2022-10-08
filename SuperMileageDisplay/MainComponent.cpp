#include "MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>

#include "Profiler.h"

//==============================================================================
MainComponent::MainComponent() :
    _speed("Vehicle MPH", 0.0f, 30.0f, Colour(253, 185, 19), 6),
    _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)),
    _map("Tracks/ShellTrack.svg", 1.0f)
{
    FUNCTION_PROFILE();
    addAndMakeVisible(_speed);
    addAndMakeVisible(_wind);
    addAndMakeVisible(_map);

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
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.alignContent = juce::Grid::AlignContent::center;
    grid.alignItems   = juce::Grid::AlignItems::center;
    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)), Track(Fr(4)), Track(Fr(1)) };
    grid.items.add(
        juce::GridItem(), juce::GridItem(_map), juce::GridItem()
    );
    grid.performLayout(getLocalBounds());
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}
