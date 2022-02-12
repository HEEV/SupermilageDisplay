#pragma once

#include <JuceHeader.h>

class Vertices {
public:
	Vertices(juce::Point<float> center, float verticalRadius, float horizontalRadius);

	// # VERTICE CALC METHODS

	juce::Point<float> getTopLeftCorner() const;
	juce::Point<float> getBottomLeftCorner() const;
	juce::Point<float> getTopRightCorner() const;
	juce::Point<float> getBottomRightCorner() const;

	// # ACCESSORS

	juce::Point<float> getCenter() const;
	void setCenter(juce::Point<float> center);

	float getVerticalRadius() const;
	void setVerticalRadius(float vRadius);

	float getHorizontalRadius() const;
	void setHorizontalRadius(float hRadius);

private:
	juce::Point<float> _center;
	float _v_radius;
	float _h_radius;
};