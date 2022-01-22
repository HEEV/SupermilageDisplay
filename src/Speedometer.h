#pragma once

//
// A speedometer JUCE widget, with which to display car data in a
// speedometer-type format.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include <JuceHeader.h>

class Speedometer
{

public:
	Speedometer(juce::Point<int> center, float min, float max, std::string name, juce::Colour color, int readoutOffset);
	Speedometer(juce::Point<int> center, float min, float max, std::string name, juce::Colour color);
	Speedometer(juce::Point<int> center, float min, float max, std::string name);
	Speedometer(juce::Point<int> center);
	~Speedometer();

	// # JUCE METHODS
	
	void draw(juce::Graphics& g);

	// # SPEEDOMETER INTERFACING

	/*
	 * Sets the minimum and maximum value which the speedometer
	 * can display.
	 *
	 * Params:
	 * min -> The minimum display value.
	 * max -> The maximum display value.
	 */	
	void setDataRange(float min, float max);

	/*
	 * Sets the data to currently display on the speedometer.
	 *
	 * Params:
	 * value -> The value to display.
	 *
	 * Throws:
	 * std::out_of_range -> If the data given is outside the range
	 * set by Speedometer::setDataRange(float, float).
	 */
	void setData(float value); // throws std::out_of_range
	
	/*
	 * Gets the data currently displaying on the speedometer.
	 *
	 * Returns:
	 * The data now displaying on the speedometer (set using
	 * Speedometer::setData(float)).
	 */
	float getData() const;

	/*
	 * Sets the name of the speedometer.
	 *
	 * Params:
	 * name -> The new name the speedometer.
	 */
	void setName(std::string name);

private:
	// # HELPER METHODS

	/*
	 * Helper method to set the rotation of the speedometer needle.
	 *
	 * Params:
	 * newRotation -> the rotation of the needle, in radians.
	 */
	void setRotation(float newRotation);



	// # DISPLAY DATA

	/*
	 * The positioning on the screen of the speedometer.
	 */
	juce::Point<int> center;

	/*
	 * The rotation value of the speedometer, in radians.
	 */
	float rotation;

	// # SPEEDOMETER DATA

	/*
	 * The minimum of the data range of the speedometer.
	 */
	float dataMin;

	/*
	 * The maximum of the data range of the speedometer.
	 */
	float dataMax;

	/*
	 * The data displaying on the speedometer.
	 */
	float data;

	/*
	 * The data displaying on the speedometer.
	 */
	std::string name;

	/*
	 * The color displaying on the speedometer.
	 */
	juce::Colour color;

	/*
	 * The offset of the data displayed
	 */
	int readoutOffset;

};
