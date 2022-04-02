/*
  ==============================================================================

    Vertices.cpp
    Created: 5 Feb 2022 11:50:41am
    Author:  ahuff

  ==============================================================================
*/

#include "Vertices.h"

Vertices::Vertices(juce::Point<float> center, float verticalRadius, float horizontalRadius) :
    _center(center), _v_radius(verticalRadius), _h_radius(horizontalRadius) {

}

void Vertices::assignCallback(std::function<void()> callback) {
    _callback = callback;
}

juce::Point<float> Vertices::getTopLeftCorner() const {
    return _center + juce::Point<float>(-_h_radius, -_v_radius);
}

juce::Point<float> Vertices::getBottomLeftCorner() const {
    return _center + juce::Point<float>(-_h_radius, _v_radius);
}

juce::Point<float> Vertices::getTopRightCorner() const {
    return _center + juce::Point<float>(_h_radius, -_v_radius);
}

juce::Point<float> Vertices::getBottomRightCorner() const {
    return _center + juce::Point<float>(_h_radius, _v_radius);
}

juce::Point<float> Vertices::getCenter() const {
    return _center;
}

void Vertices::setCenter(juce::Point<float> center) {
    _center = center;
    runCallback();
}

float Vertices::getVerticalRadius() const {
    return _v_radius;
}

void Vertices::setVerticalRadius(float vRadius) {
    _v_radius = vRadius;
    runCallback();
}

float Vertices::getHorizontalRadius() const {
    return _h_radius;
}

void Vertices::setHorizontalRadius(float hRadius) {
    _h_radius = hRadius;
    runCallback();
}

void Vertices::runCallback() {
    if (_callback) {
        _callback();
    }
}