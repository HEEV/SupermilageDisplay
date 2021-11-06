#pragma once

#include <JuceHeader.h>

class Speedometer
{

public:
	Speedometer(juce::Point<int> center, float min, float max);
	Speedometer(juce::Point<int> center);
	~Speedometer();

	// # JUCE METHODS
	
	void draw(juce::Graphics& g);

	// # SPEEDOMETER INTERFACING
	
	void setDataRange(float min, float max);
	void setData(float value); // throws std::out_of_range
	float getData() const;

private:
	void setRotation(float newRotation);

	// # DISPLAY DATA

	juce::Point<int> center;
	float rotation;

	// # SPEEDOMETER DATA

	float dataMin;
	float dataMax;
	float data;

};
