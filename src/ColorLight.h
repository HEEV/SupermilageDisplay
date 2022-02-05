#pragma once

//
// A speedometer JUCE widget, with which to display car data in a
// speedometer-type format.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include <JuceHeader.h>

class ColorLight {
public:

	ColorLight(juce::Point<int> center, juce::Colour colorStart, std::string name);
	ColorLight(juce::Point<int> center);
	~ColorLight();

	// # JUCE METHODS

	void draw(juce::Graphics& g);

	// # SPEEDOMETER INTERFACING

	/*
	 * Sets the light's color
	 *
	 * Params:
	 * color -> The color to display.
	 */
	void setColor(juce::Colour color); // throws std::out_of_range

	/*
	 * Gets the light's color.
	 *
	 * Returns:
	 * The light's current color, which is set using setColor(juce::Colour color)
	 */
	juce::Colour getColor() const;

	/*
	 * Sets the name of the light.
	 *
	 * Params:
	 * name -> The new name the light.
	 */
	void setName(std::string name);

private:

	// # DISPLAY DATA

	/*
	 * The positioning on the screen of the light.
	 */
	juce::Point<int> center;

	/*
	 * The light's color
	 */
	juce::Colour color;

	/*
	 * The data displaying on the speedometer.
	 */
	std::string name;
};