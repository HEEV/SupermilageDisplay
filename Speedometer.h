#pragma once

#include <JuceHeader.h>

class Speedometer
{

public:
	Speedometer(juce::Point<int> center);
	~Speedometer();

	void draw(juce::Graphics& g);
	void setRotation(float newRotation);
	void setDataRange(float min, float max);
	void updateDataValue(float value);

private:
	juce::Point<int> center;
	float rotation;
	float dataMin;
	float dataMax;
};