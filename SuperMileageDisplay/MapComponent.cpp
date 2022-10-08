#include "MapComponent.h"
#include <fmt/format.h>
#include <iostream>
#include <string>

#include "Profiler.h"
#include "Constants.h"

MapComponent::MapComponent(String trackFilepath, float mapLength) : 
    _track(), _trackLength(mapLength), _trackerPos(0.0f, 0.0f), _stroke(5.0f, PathStrokeType::JointStyle::curved)
{
    FUNCTION_PROFILE();
    auto svg = Drawable::createFromSVGFile(trackFilepath);
    setFramesPerSecond(30);
    
    if(svg == nullptr)
        throw std::runtime_error("Failed to create track object with SVG " + trackFilepath.toStdString());
    
    _track = svg->getOutlineAsPath();
    updateDistance(0.0f);
}

MapComponent::~MapComponent()
{
}

void MapComponent::paint(juce::Graphics& g)
{
    FUNCTION_PROFILE();
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
    g.strokePath(_track, _stroke);
    g.setColour(Colours::red);
    float width = _stroke.getStrokeThickness();
    g.fillEllipse(_trackerPos.x - width / 2.0f, _trackerPos.y - width / 2.0f, width, width);
    g.setColour(Colours::black);
}

void MapComponent::resized()
{
    FUNCTION_PROFILE();
    auto bounds = getBounds().toFloat();
    _track.scaleToFit
    (
        _stroke.getStrokeThickness() / 2.0f, 
        _stroke.getStrokeThickness() / 2.0f, 
        bounds.getWidth() - _stroke.getStrokeThickness(), 
        bounds.getHeight() - _stroke.getStrokeThickness(), 
        true
    );
}

void MapComponent::update()
{
    static float distance = 0.0f;
    updateDistance(distance);
    distance += 0.005f;
    if(distance > _trackLength)
        distance = 0.0f;
}

void MapComponent::updateDistance(float dist)
{
    FUNCTION_PROFILE();
    float distanceAlong = std::min(dist, _trackLength);
    distanceAlong = std::max(distanceAlong, 0.0f);
    float graphicLength = _track.getLength();
    _trackerPos = _track.getPointAlongPath(distanceAlong / _trackLength * graphicLength);
}
