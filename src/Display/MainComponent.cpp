#include "Display/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>

#include "Profiler/Profiler.h"

//==============================================================================
MainComponent::MainComponent() :
    _speed("Vehicle MPH", 0.0f, 30.0f, Colour(253, 185, 19), 6),
    _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)),
    _map("Tracks/ShellTrack.svg", 1.0f),
    _tilt(10.0f),
    _timer(),
    _counter(1.0, 4),
    _engTemp(0.0f, 90.0f, 9),
    _volt(10.0f, 13.0f, 3)
{
    FUNCTION_PROFILE();
    addAndMakeVisible(_speed);
    addAndMakeVisible(_wind);
    addAndMakeVisible(_map);
    addAndMakeVisible(_tilt);
    addAndMakeVisible(_timer);
    addAndMakeVisible(_counter);
    addAndMakeVisible(_engTemp);
    addAndMakeVisible(_volt);

    _speed.setData(15.0f);
    _wind.setData(20.0f);
    _engTemp.setData(45.0f);
    _volt.setData(12.0f);

    addMouseListener(&_mouse, true);
    
    setSize(getParentWidth(), getParentHeight());
    setFramesPerSecond(30);
}

void MainComponent::update()
{
    static float randSpeed = 0.0f;
    static float randWind = 0.0f;

    _map.incDistance(0.01f);
    _counter.incDistanceTraveled(0.01f);
    _speed.setData(15.0f + randSpeed);
    _wind.setData(20.0f + randWind);

    Random& rand = Random::getSystemRandom();
    randSpeed += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
    randWind += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
    _tilt.setCurrentTilt(rand.nextFloat() / 2.0f);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
//Function exicuts every frame
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainComponent::resized()
{
    FUNCTION_PROFILE();
    FlexBox speed;
    speed.alignContent = FlexBox::AlignContent::center;
    speed.alignItems = FlexBox::AlignItems::center;
    speed.justifyContent = FlexBox::JustifyContent::center;

    speed.items.add(FlexItem(_speed).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));
    speed.items.add(FlexItem(_wind).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));

    FlexBox vert;
    vert.alignContent = FlexBox::AlignContent::center;
    vert.alignItems = FlexBox::AlignItems::center;
    vert.justifyContent = FlexBox::JustifyContent::center;
    vert.flexDirection = FlexBox::Direction::column;

    vert.items.add(FlexItem(_tilt).withMinWidth(500.0f).withMinHeight(50.0f).withMargin(5.0f));
    vert.items.add(FlexItem(_timer).withMinWidth(250.0f).withMinHeight(60.0f));

    FlexBox horz;
    horz.alignContent = FlexBox::AlignContent::center;
    horz.alignItems = FlexBox::AlignItems::center;
    horz.justifyContent = FlexBox::JustifyContent::center;

    horz.items.add(FlexItem(vert).withFlex(1.0f).withMinWidth(500.0f).withMinHeight(250.0f));
    horz.items.add(FlexItem(_map).withMinWidth(300.0f).withMinHeight(250.0f));

    FlexBox vert2;
    vert2.alignContent = FlexBox::AlignContent::center;
    vert2.alignItems = FlexBox::AlignItems::center;
    vert2.justifyContent = FlexBox::JustifyContent::center;
    vert2.flexDirection = FlexBox::Direction::column;
    
    vert2.items.add(FlexItem(horz).withFlex(1.0f));
    vert2.items.add(FlexItem(speed).withFlex(1.5f));
    vert2.items.add(FlexItem(_counter).withMinWidth(getWidth() - 100.0f).withMinHeight(125.0f));

    FlexBox horz2;
    horz2.alignContent = FlexBox::AlignContent::center;
    horz2.alignItems = FlexBox::AlignItems::center;
    horz2.justifyContent = FlexBox::JustifyContent::center;

    horz2.items.add(FlexItem(_engTemp).withMinHeight(getHeight()).withMinWidth(50.0f));
    horz2.items.add(FlexItem(vert2).withFlex(1.0f).withMinHeight(getHeight()));
    horz2.items.add(FlexItem(_volt).withMinHeight(getHeight()).withMinWidth(50.0f));

    horz2.performLayout(getLocalBounds());
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}