#pragma once

//
// A speedometer JUCE widget, with which to display car data in a
// speedometer-type format.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include <JuceHeader.h>
#include <atomic>

class ColorLight : public Component
{
public:

	ColorLight(std::string name, Colour color);
	~ColorLight();

	// # JUCE METHODS

	void paint(juce::Graphics& g) override;

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
	 * The light's color
	 */
	std::atomic<juce::Colour> _color;

	/*
	 * The data displaying on the speedometer.
	 */
	std::string _name;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorLight)
};