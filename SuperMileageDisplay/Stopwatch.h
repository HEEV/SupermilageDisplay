#pragma once
//
// A Stopwatch JUCE widget, with which to display the time since startup
//
// Author:	Ian Hubble
// Date:	February 19, 2022
//

#include <JuceHeader.h>
#include <chrono>

class Stopwatch {
public:
	Stopwatch(juce::Point<int> center, std::string name);
	Stopwatch();
	~Stopwatch();

	// # JUCE METHODS

	void draw(juce::Graphics& g);

	// # CLOCK INTERFACING

	/*
	 * Gets the clock itself
	 */
	void updateTimeSinceStartup();

	/*
	 * Resets the clock
	 */
	void resetClock();

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
	 * The positioning on the screen of the clock.
	 */
	juce::Point<int> center;

	/*
	 * The time points needed for calculating minutes and seconds
	 */
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> currentTime;

	/*
	 * The clock's time values
	 */
	int seconds;
	int minutes;

	/*
	 * The data displaying on the speedometer.
	 */
	std::string name;

	/*
	 * The extra 0 when seconds is less than 10
	 */
	std::string extraZero;
};
