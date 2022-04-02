#pragma once

//
// A speedometer JUCE widget, with which to display car data in a
// speedometer-type format.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include "ISizeable.h"
#include <JuceHeader.h>

class SizeableSpeedometer : public ISizeable
{

public:
	SizeableSpeedometer(Vertices vertices, float min, float max, std::string name, juce::Colour color, int readoutOffset);
	~SizeableSpeedometer();

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
	 * The dimensional data of the speedometer.
	 */
	Vertices _vertices;

	/*
	 * The rotation value of the speedometer, in radians.
	 */
	float _rotation;

	// # SPEEDOMETER DATA

	/*
	 * The minimum of the data range of the speedometer.
	 */
	float _dataMin;

	/*
	 * The maximum of the data range of the speedometer.
	 */
	float _dataMax;

	/*
	 * The data displaying on the speedometer.
	 */
	float _data;

	/*
	 * The data displaying on the speedometer.
	 */
	std::string _name;

	/*
	 * The color displaying on the speedometer.
	 */
	juce::Colour _color;

	/*
	 * The offset of the data displayed
	 */
	int _readoutOffset;

};
