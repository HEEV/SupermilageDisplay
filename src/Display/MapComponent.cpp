#include "Display/MapComponent.h"
#include <fmt/format.h>
#include <iostream>
#include <string>

#include "Profiler/Profiler.h"
#include "Display/Constants.h"

MapComponent::MapComponent(String trackFilepath, float mapLength) : 
    _track(), _trackLength(mapLength), _trackerPos(0.0f, 0.0f), 
    _lastPos(0.0f, 0.0f), _stroke(5.0f, PathStrokeType::JointStyle::curved), 
    _distanceAlong(0.0f), _arrowLength(0.0f)
{
    FUNCTION_PROFILE();
    auto svg = Drawable::createFromSVGFile(File::getCurrentWorkingDirectory().getChildFile(trackFilepath));
    
    if(svg == nullptr)
        throw std::runtime_error("Failed to create track object with SVG " + trackFilepath.toStdString());
    
    _track = svg->getOutlineAsPath();
    _lastPos = _track.getPointAlongPath(0.0f);
    _trackerPos = _track.getPointAlongPath(0.1f);
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
    Line<float> ar(_lastPos, _trackerPos);
    float curLength = ar.getLength();
    ar = ar.withShortenedEnd((curLength - _arrowLength) / 2.0f).withShortenedStart((curLength - _arrowLength) / 2.0f);
    g.drawArrow(ar, width, width * 2.5f, width * 2.5f);

    g.setColour(Colours::green);
    auto start = _track.getPointAlongPath(0.0f);
    Rectangle<float> startLine;
    startLine.setWidth(width);
    startLine.setHeight(width * 5.0f);
    startLine.setCentre(start);
    g.fillRect(startLine);

    g.setColour(Colours::black);
}

void MapComponent::resized()
{
    FUNCTION_PROFILE();
    auto bounds = getBounds().toFloat();
    _arrowLength = 0.1f * bounds.getWidth();
    float margin = _arrowLength / 2.0f;
    _track.scaleToFit
    (
        (_stroke.getStrokeThickness() + margin) / 2.0f,
        (_stroke.getStrokeThickness() + margin) / 2.0f,
        bounds.getWidth() - _stroke.getStrokeThickness() - margin,
        bounds.getHeight() - _stroke.getStrokeThickness() - margin, 
        true
    );
}

void MapComponent::updateDistance(float dist)
{
    FUNCTION_PROFILE();
    _distanceAlong = std::min(dist, _trackLength);
    _distanceAlong = std::max(_distanceAlong, 0.0f);
    float graphicLength = _track.getLength();
    _lastPos = _trackerPos;
    _trackerPos = _track.getPointAlongPath(_distanceAlong / _trackLength * graphicLength);
    repaint();
}

void MapComponent::incDistance(float deltaDist)
{
    FUNCTION_PROFILE();
    if (_distanceAlong + deltaDist > _trackLength)
        updateDistance(_trackLength - _distanceAlong + deltaDist);
    else if (_distanceAlong + deltaDist < 0.0f)
        updateDistance(_trackLength + _distanceAlong + deltaDist);
    else
        updateDistance(_distanceAlong + deltaDist);
}

void MapComponent::reset()
{
    _distanceAlong = 0;
}
