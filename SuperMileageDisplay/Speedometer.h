#pragma once

#include <JuceHeader.h>
#include <atomic>
#include <thread>

#include "LapCounter.h"

/**
 * A speedometer JUCE widget, with which to display car data in a
 * speedometer-type format
 * .
 * @implements AnimatedAppComponent
 */
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

	/**
	 * @brief Sets the minimum and maximum value which the speedometer
	 * can display.
	 *
	 * @param min The minimum display value.
	 * @param max The maximum display value.
	 */	
	void setDataRange(float min, float max);

	/**
	 * @brief The data to currently display on the speedometer. 
	 * Data outside of the range will be set to the closest
	 * extreme.
	 * 
	 * @param value -> The value to display
	 *
	 */
	void setData(float value); // throws std::out_of_range
	
	/**
	 * @brief The data currently displaying on the speedometer.
	 *
	 * 
	 * @return data now displaying on the speedometer (set using
	 * Speedometer::setData(float)).
	 */
	float getData() const;

	/**
	 * @brief Sets the name of the speedometer.
	 *
	 * @param name The new name the speedometer.
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
