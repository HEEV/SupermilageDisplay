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
#include <thread>

#include "LapCounter.h"

class Speedometer : public AnimatedAppComponent
{
public:
	Speedometer(std::string_view name, float minData, float maxData, juce::Colour color, int subdivisions = 8, int lineWidth = 5);
	~Speedometer();

	// # JUCE METHODS
	
	void paint(juce::Graphics& g) override;
	void resized() override { /*do nothing*/ }
	void update() override;

	void addLapCounter(LapCounter* lc);

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
	// # DISPLAY DATA
	int _lineWidth;
	int _subdivisions;

	LapCounter* _lc;

	/*
	 * The rotation value of the speedometer, in radians.
	 */
	std::atomic<float> _rotation;

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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Speedometer)
};
