#include "Speedometer.h"

#include "Constants.h"
#include <stdexcept>

Speedometer::Speedometer(juce::Point<int> center, float min, float max) {
	this->center = center;
	setDataRange(min, max);

Speedometer::Speedometer(juce::Point<int> center):
       Speedometer(center, 0, 100)	{
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
	juce::Rectangle<float> house(center.x - 10, center.y - 10, 20, 20);
	g.setColour(juce::Colours::royalblue);
	g.fillEllipse(house);

	juce::Path path;
	path.startNewSubPath(juce::Point<float>(center.x + 10 * std::cos(rotation + 1.57075), center.y + 10 * std::sin(rotation + 1.57075)));
	path.lineTo(juce::Point<float>(center.x + 60 * std::cos(rotation), center.y + 60 * std::sin(rotation)));
	path.lineTo(juce::Point<float>(center.x - 10 * std::cos(rotation + 1.57075), center.y - 10 * std::sin(rotation + 1.57075)));
	path.closeSubPath();

	g.fillPath(path);
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
	// setRotation(3.1415 * (dataMax + value) / dataMax);
	
	if (value < dataMin || value > dataMax) {
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
