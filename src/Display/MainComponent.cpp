#include "Display/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>

#include "Profiler/Profiler.h"

//=========== TODO LIST (yaay...) ==============================================
//  X. Coolant temp
//  X. Engine bay / Intake temp
//  3. Count down bar
//  4. Compass
//  5. Pendulum gyroscope
//  6. Map points north up
//  7. Driver bay temp
//  8. Wind direction arrow
//  X. Red/Yellow/Green burn lights
//  X. Kill switch lights
// 11. Whole background fades to red when any guage reaches extreme
//  X. Bigger / more labels
//  X. Fix bug with label truncating on temp bars
// 14. Fix bug with map updating wrong and excepting
// 15. Figure out how on Earth we are gonna fit all of this on the screen
//============================================================================== 

//==============================================================================
MainComponent::MainComponent() :
    _speed("Vehicle MPH", 0.0f, 30.0f, 25.0f, 6),
    _wind("Wind MPH", 0.0f, 40.0f, 35.0f),
    _map("Tracks/ShellTrack.svg", 1.0f),
    _tilt(3.1415f / 12.0f),
    _timer(),
    _counter(1.0, 4),
    _engTemp(0.0f, 90.0f, 9, 'E', 80.0f),
    _coolTemp(0.0f, 110.0f, 9, 'C', 100.0f),
    _intakeTemp(0.0f, 110.0f, 9, 'I', 100.0f),
    _volt(10.0f, 13.0f, 3, 'V', 12.5f),
    _burnLight(),
    _killLight()

{
    FUNCTION_PROFILE();
    addAndMakeVisible(_speed);
    addAndMakeVisible(_wind);
    addAndMakeVisible(_map);
    addAndMakeVisible(_tilt);
    addAndMakeVisible(_timer);
    addAndMakeVisible(_counter);
    addAndMakeVisible(_engTemp);
    addAndMakeVisible(_coolTemp);
    addAndMakeVisible(_intakeTemp);
    addAndMakeVisible(_volt);
    addAndMakeVisible(_burnLight);
    addAndMakeVisible(_killLight);

    _speed.setData(15.0f);
    _wind.setData(20.0f);
    _engTemp.setData(80.0f);
    _coolTemp.setData(100.0f);
    _intakeTemp.setData(100.0f);
    _volt.setData(12.5f);

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


    if (!countStarted) {
        _burnLight.burn(5000);
        countStarted = true;
    }
}

MainComponent::~MainComponent()
{
}

//==============================================================================
//Function exicuts every frame
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
    //g.fillAll(getLookAndFeel().setColour(juce::Colours::darkgrey.withAlpha(0.5f));
    //g.fillAll(juce::Colours::cornflowerblue.withAlpha(0.5f));
}

void MainComponent::resized()
{
    FUNCTION_PROFILE();

    //vert.items.add(FlexItem(_timer).withMinWidth(250.0f).withMinHeight(60.0f));

    //spedomiters 
    FlexBox speed;
    speed.alignContent = FlexBox::AlignContent::center;
    speed.alignItems = FlexBox::AlignItems::center;
    speed.justifyContent = FlexBox::JustifyContent::center;
//<<<<<<< HEAD
    
    //speed.items.add(FlexItem(_speed).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));
    //speed.items.add(FlexItem(_wind).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));
    //speed.items.add(FlexItem(_speed).withMinWidth(300.0f).withMinHeight(300.0f).withMargin(1.0f));
    //speed.items.add(FlexItem(_tilt).withMinWidth(200.0f).withMinHeight(140.0f));
    //speed.items.add(FlexItem(_wind).withMinWidth(300.0f).withMinHeight(300.0f).withMargin(1.0f));   
        
    //speed.items.add(FlexItem(_wind).;
//=======

    FlexBox vert1;
    vert1.alignContent = FlexBox::AlignContent::center;
    vert1.alignItems = FlexBox::AlignItems::center;
    vert1.justifyContent = FlexBox::JustifyContent::center;
    vert1.flexDirection = FlexBox::Direction::column;

    vert1.items.add(FlexItem(_burnLight).withMinWidth(200.0f).withMinHeight(60.0f).withMargin(5.0f));
    vert1.items.add(FlexItem(_tilt).withMinWidth(200.0f).withMinHeight(140.0f));
    vert1.items.add(FlexItem(_timer).withMinWidth(200.0f).withMinHeight(60.0f).withMargin(5.0f));
    vert1.items.add(FlexItem(_killLight).withMinWidth(200.0f).withMinHeight(60.0f).withMargin(5.0f));

    //speed.items.add(FlexItem(_speed).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));
    //speed.items.add(FlexItem(_wind).withMinWidth(250.0f).withMinHeight(250.0f).withMargin(5.0f));
    
    speed.items.add(FlexItem(_speed).withMinWidth(300.0f).withMinHeight(350.0f).withMargin(1.0f));
    speed.items.add(FlexItem(vert1).withMinWidth(200.0f).withMinHeight(140.0f));
    speed.items.add(FlexItem(_wind).withMinWidth(300.0f).withMinHeight(350.0f).withMargin(1.0f));    
//>>>>>>> 63ab22b42982da840e06a4bddd07f5b40c0ff53a
    

    //horz.items.add(FlexItem(_map).withMinWidth(300.0f).withMinHeight(250.0f));

    FlexBox horz1;
    horz1.alignContent = FlexBox::AlignContent::center;
    horz1.alignItems = FlexBox::AlignItems::center;
    horz1.justifyContent = FlexBox::JustifyContent::center;

    horz1.items.add(FlexItem(_map).withMinWidth(250.0f).withMinHeight(200.0f).withMargin(10.0f));
    horz1.items.add(FlexItem(_counter).withMinWidth(getWidth() - 500.0f).withMinHeight(100.0f));

    FlexBox vert2;
    vert2.alignContent = FlexBox::AlignContent::spaceAround;
    vert2.alignItems = FlexBox::AlignItems::stretch;
    vert2.justifyContent = FlexBox::JustifyContent::spaceAround;
    vert2.flexDirection = FlexBox::Direction::column;
    
    vert2.items.add(FlexItem(speed).withFlex(0.5f));
    //vert2.items.add(FlexItem(_timer).withMinWidth(500.0f).withMinHeight(30.0f).withMargin(5.0f));
    vert2.items.add(FlexItem(horz1).withMinWidth(getWidth() - 500.0f).withMinHeight(100.0f).withMargin(30.0f));

    FlexBox horz2;
    horz2.alignContent = FlexBox::AlignContent::center;
    horz2.alignItems = FlexBox::AlignItems::center;
    horz2.justifyContent = FlexBox::JustifyContent::center;

    horz2.items.add(FlexItem(_engTemp).withMinHeight(getHeight()).withMinWidth(50.0f));
    horz2.items.add(FlexItem(_coolTemp).withMinHeight(getHeight()).withMinWidth(50.0f));
    horz2.items.add(FlexItem(vert2).withFlex(1.0f).withMinHeight(getHeight()));
    horz2.items.add(FlexItem(_volt).withMinHeight(getHeight()).withMinWidth(50.0f));
    horz2.items.add(FlexItem(_intakeTemp).withMinHeight(getHeight()).withMinWidth(50.0f));

    horz2.performLayout(getLocalBounds());
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}
