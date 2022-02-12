#pragma once

//
// A speedometer JUCE widget, with which to display car data in a
// speedometer-type format.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include <JuceHeader.h>
#include <functional>
#include <string>

typedef std::function<void()> Callback;

class ProgressBar {
public:
	ProgressBar(juce::Point<int> center, std::string name, unsigned int currentVal, juce::Colour, juce::Colour, unsigned int maxVal);
	ProgressBar(juce::Point<int> center, std::string name, unsigned int currentVal);
	~ProgressBar();

	// # JUCE METHODS

	void draw(juce::Graphics& g);

	// # ACCESSORS

	/*
	* Sets the maximum value of the progress bar.
	* 
	* Params:
	* maxVal -> the maximum value.
	*/
	void setMaxValue(unsigned int maxVal);

	/*
	* Sets the current value of the progress bar.
	* 
	* Params:
	* val -> The new value.
	*/
	void setValue(unsigned int val);

	/*
	* Sets a callback function for when the bar is full.
	*/
	void setOnFull(Callback callback);

	/*
	* Gets the maximum value of the progress bar.
	*/
	unsigned int getMaxValue() const;

	/*
	* Gets the current value of the progress bar.
	*/
	unsigned int getValue() const;

private:
	juce::Point<int> _center;

	unsigned int _max_val;
	unsigned int _val;
	Callback _on_full;
	std::string _name;

	juce::Colour _fill_color;
	juce::Colour _empty_color;

	void _checkFull();
	
};
