
#include "Speedometer.h"
#include "Constants.h"
#include <stdexcept>

Speedometer::Speedometer(std::string_view name, float minData, float maxData, juce::Colour color) : _name(name), _dataMin(minData), _dataMax(maxData), _color(color)
{
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
void Speedometer::paint(juce::Graphics& g) 
{
	auto bounds = getLocalBounds();

	g.setColour(juce::Colours::yellow);
	g.drawRect(bounds.getCentreX() - 100, bounds.getCentreY() - 80, 200, 130);
	juce::Font theFont("Consolas", 20.0f, juce::Font::bold);
	g.setFont(theFont);
	g.drawText(_name, bounds.getCentreX() - 100, bounds.getCentreY() + 10, 200, 20, juce::Justification::centred, false);

	g.setColour(_color);
	juce::Rectangle<float> house(bounds.getCentreX() - 10, bounds.getCentreY() - 10, 20, 20);
	g.fillEllipse(house);

	juce::Path path;
	path.startNewSubPath(juce::Point<float>(bounds.getCentreX() + 10 * std::cos(_rotation + 1.57075), bounds.getCentreY() + 10 * std::sin(_rotation + 1.57075)));
	path.lineTo(juce::Point<float>(			bounds.getCentreX() + 60 * std::cos(_rotation),			  bounds.getCentreY() + 60 * std::sin(_rotation)));
	path.lineTo(juce::Point<float>(			bounds.getCentreX() - 10 * std::cos(_rotation + 1.57075), bounds.getCentreY() - 10 * std::sin(_rotation + 1.57075)));
	path.closeSubPath();
	g.fillPath(path);

	g.setColour(juce::Colours::white);
	g.drawText(std::to_string(_data),	  bounds.getCentreX() - 25, bounds.getCentreY() + 30, 50, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(_dataMin),					  bounds.getCentreX() - 80, bounds.getCentreY() - 10, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(_dataMin + (_dataMax * .25)), bounds.getCentreX() - 60, bounds.getCentreY() - 60, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(_dataMin + (_dataMax * .5)),  bounds.getCentreX() - 10, bounds.getCentreY() - 80, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(_dataMin + (_dataMax * .75)), bounds.getCentreX() + 40, bounds.getCentreY() - 60, 20, 20, juce::Justification::centred, false);
	g.drawText(std::to_string(_dataMax),					  bounds.getCentreX() + 60, bounds.getCentreY() - 10, 20, 20, juce::Justification::centred, false);
}


/**
 * Sets the range of data points in the speedometer.
 *
 * @param min The minimum speedometer value.
 * @param max The maximum speedometer value.
 */
void Speedometer::setDataRange(float min, float max) {
	this->_dataMin = min;
	this->_dataMax = max;
}

/**
 * Sets the speedometer value.
 *
 * @param The speedometer value.
 * @throws std::out_of_range
 */
void Speedometer::setData(float value) {
	if (value < _dataMin || value > _dataMax) {
		throw std::out_of_range("value must be within the provided data range.");
	}

	_data = value;

	// Range the data around 0
	float weight = _dataMax - _dataMin;
	value = value - _dataMin;

	// Set the rotation
	_rotation = PI * (weight + value) / weight;
	
}

/**
 * Gets the data that is displaying on the speedometer.
 *
 * @return The speedometer data.
 */
float Speedometer::getData() const {
	return _data;
}

/*
 * Sets the name of the speedometer.
 *
 * Params:
 * name -> The new name the speedometer.
 */
void Speedometer::setName(std::string name) {
	this->_name = name;
}