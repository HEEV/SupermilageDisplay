#include "Display/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>
#include <Packets.h>

#include "Profiler/Profiler.h"

//==============================================================================
MainComponent::MainComponent() :
    _speed("Vehicle MPH", 0.0f, 30.0f, Colour(253, 185, 19), 6),
    _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)),
    _map("Tracks/ShellTrack.svg", 1.0f),
    _tilt(3.1415f/12.0f),
    _timer(),
    _counter(1.0, 4),
    _engTemp(0.0f, 90.0f, 9),
    _volt(10.0f, 13.0f, 3),
    _manager("10.12.11.32:25565")
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

    addMouseListener(&_mouse, true);
    
    setSize(getParentWidth(), getParentHeight());

    REGISTER_TYPE_TO_MANAGER(WheelData, "vel", _manager);
    REGISTER_TYPE_TO_MANAGER(BatteryVoltage, "bat", _manager);
    REGISTER_TYPE_TO_MANAGER(EngineTemp, "enTemp", _manager);
    REGISTER_TYPE_TO_MANAGER(GPSPosition, "gps", _manager);
    REGISTER_TYPE_TO_MANAGER(WindSpeed, "wind", _manager);
    REGISTER_TYPE_TO_MANAGER(CarTilt, "tilt", _manager);

    _manager.addDataReader("vel", std::function([this](WheelData* v){
        _speed.setData(v->velocity());
        _map.incDistance(v->distTravelled());
    }));
    _manager.addDataReader("bat", std::function([this](BatteryVoltage* bat){
        _volt.setData(bat->volt());
    }));
    _manager.addDataReader("enTemp", std::function([this](EngineTemp* temp) {
        _engTemp.setData(temp->temp());
    }));
    _manager.addDataReader("wind", std::function([this](WindSpeed* wind) {
        _wind.setData(wind->headSpeed());
    }));
    _manager.addDataReader("tilt", std::function([this](CarTilt* tlt){
        _tilt.setCurrentTilt(tlt->angle());
    }));

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

    vert1.items.add(FlexItem(_tilt).withMinWidth(200.0f).withMinHeight(140.0f));
    vert1.items.add(FlexItem(_timer).withMinWidth(200.0f).withMinHeight(60.0f).withMargin(5.0f));

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

    horz1.items.add(FlexItem(_map).withMinWidth(250.0f).withMinHeight(200.0f).withMargin(50.0f));
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
    horz2.items.add(FlexItem(vert2).withFlex(1.0f).withMinHeight(getHeight()));
    horz2.items.add(FlexItem(_volt).withMinHeight(getHeight()).withMinWidth(50.0f));

    horz2.performLayout(getLocalBounds());
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}
