#include "Speedometer.h"

#include "Constants.h"
#include <stdexcept>


Speedometer::Speedometer(juce::Point<int> center, float min, float max, std::string name, juce::Colour color, int readoutOffset) {
	this->center = center;
	this->color = color;
	this->readoutOffset = readoutOffset;
	setDataRange(min, max);
	setName(name);
}

Speedometer::Speedometer(juce::Point<int> center, float min, float max, std::string name, juce::Colour color) :
	Speedometer(center, min, max, name, color, 0) {
}

Speedometer::Speedometer(juce::Point<int> center, float min, float max, std::string name):
	Speedometer(center, min, max, name, juce::Colours::yellow, 0) {
}

Speedometer::Speedometer(juce::Point<int> center):
    Speedometer(center, 0, 100, "Name", juce::Colours::yellow, 0)	{
}

/**
 * Destructor.
 */
Speedometer::~Speedometer() {

}

/**
 * Draws the speedometer within the graphics context.
 *
 * @param g The JUCE graphics context.
 */
void Speedometer::draw(juce::Graphics& g) {
	g.setColour(juce::Colours::yellow);
	g.drawRect(center.x - 100, center.y - 80, 200, 130);
	juce::Font theFont("Consolas", 20.0f, juce::Font::bold);
	g.setFont(theFont);
	g.drawText(name, center.x - 100, center.y + 10, 200, 20, juce::Justification::centred, false);

	g.setColour(color);
	juce::Rectangle<float> house(center.x - 10, center.y - 10, 20, 20);
	g.fillEllipse(house);

	juce::Path path;
	path.startNewSubPath(juce::Point<float>(center.x + 10 * std::cos(rotation + 1.57075), center.y + 10 * std::sin(rotation + 1.57075)));
	path.lineTo(juce::Point<float>(			center.x + 60 * std::cos(rotation),			  center.y + 60 * std::sin(rotation)));
	path.lineTo(juce::Point<float>(			center.x - 10 * std::cos(rotation + 1.57075), center.y - 10 * std::sin(rotation + 1.57075)));
	path.closeSubPath();
	g.fillPath(path);

	g.setColour(juce::Colours::white);
	g.drawText(std::to_string(data),	  center.x - 25 + readoutOffset, center.y + 30, 50, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(dataMin),					  center.x - 80, center.y - 10, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(dataMin + (dataMax * .25)), center.x - 60, center.y - 60, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(dataMin + (dataMax * .5)),  center.x - 10, center.y - 80, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(dataMin + (dataMax * .75)), center.x + 40, center.y - 60, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(dataMax),					  center.x + 60, center.y - 10, 20, 20, juce::Justification::centred, false);
}

/**
 * Sets the rotation of the speedometer pin, from a range of
 * 0 radians to 2pi radians.
 *
 * @param newRotation the radian value.
 *
 **/
void Speedometer::setRotation(float newRotation) {
	this->rotation = newRotation;
}

/**
 * Sets the range of data points in the speedometer.
 *
 * @param min The minimum speedometer value.
 * @param max The maximum speedometer value.
 */
void Speedometer::setDataRange(float min, float max) {
	this->dataMin = min;
	this->dataMax = max;
}

/**
 * Sets the speedometer value.
 *
 * @param The speedometer value.
 * @throws std::out_of_range
 */
void Speedometer::setData(float value) {
	if ((int)value < dataMin || (int)value > dataMax) {
		throw std::out_of_range("value must be within the provided data range.");
	}

	data = value;

	// Range the data around 0
	float weight = dataMax - dataMin;
	value = value - dataMin;

	// Set the rotation
	setRotation(PI * (weight + value) / weight);
	
}

/**
 * Gets the data that is displaying on the speedometer.
 *
 * @return The speedometer data.
 */
float Speedometer::getData() const {
	return data;
}

/*
 * Sets the name of the speedometer.
 *
 * Params:
 * name -> The new name the speedometer.
 */
void Speedometer::setName(std::string name) {
	this->name = name;
}
