
#include "Speedometer.h"
#include "Constants.h"
#include <stdexcept>
#include <sstream>

Speedometer::Speedometer(std::string_view name, float minData, float maxData, juce::Colour color, int subdivisions, int lineWidth) :
	_name(name), _dataMin(minData), _dataMax(maxData), _color(color), _lineWidth(lineWidth), _subdivisions(subdivisions)
{
	setData(_dataMin);
}

/**
 * Destructor.
 */
Speedometer::~Speedometer() {

}

/**
 * Draws the speedometer within the graphics context.
 * Do NOT call - called by JUCE
 *
 * @param g The JUCE graphics context.
 */
void Speedometer::paint(juce::Graphics& g) 
{
	//setPaintingIsUnclipped(true);
	auto bounds = getLocalBounds();
	Font f("Consolas", 20.0f, Font::bold);
	g.setFont(f);

	g.setColour(Colours::black);

	//Create semicircle for speedomoter
	Path arc;
	float radius = bounds.getWidth() / 2.0f - 2 * _lineWidth;
	arc.addCentredArc(bounds.getCentreX(), bounds.getBottom() - 2 * f.getHeight(), radius, radius, 0.0f, -PI / 2, PI / 2, true);

	//Add tickmarks to arc
	for (int i = 0; i < _subdivisions + 1; i++)
	{
		float x1 = (radius + 1.5 * _lineWidth) * cos(i * PI / _subdivisions) + bounds.getCentreX();
		float y1 = (radius + 1.5 * _lineWidth) * sin(-i * PI / _subdivisions) + bounds.getBottom() - 2 * f.getHeight();
		float x2 = (radius - 1.5 * _lineWidth) * cos(i * PI / _subdivisions) + bounds.getCentreX();
		float y2 = (radius - 1.5 * _lineWidth) * sin(-i * PI / _subdivisions) + bounds.getBottom() - 2 * f.getHeight();

		std::stringstream label;
		label.precision(0);
		label << std::fixed << _dataMax - i * (_dataMax - _dataMin) / _subdivisions;

		float textX = (radius - _lineWidth * _lineWidth) * cos(i * PI / _subdivisions) + bounds.getCentreX();
		float textY = (radius - _lineWidth * _lineWidth) * sin(-i * PI / _subdivisions) + bounds.getBottom() - 2 * f.getHeight();
		Rectangle<int> textPos;
		textPos.setWidth(f.getStringWidthFloat(label.str()));
		textPos.setHeight(f.getHeight());
		textPos.setCentre(textX, textY);

		g.drawFittedText(label.str(), textPos, Justification::centred, 1);

		arc.startNewSubPath(x1, y1);
		arc.lineTo(x2, y2);
	}

	g.setColour(_color);
	g.strokePath(arc, PathStrokeType(_lineWidth));

	Path needle;
	float needleEndX = (radius + 3 * _lineWidth) * cos(_rotation) + bounds.getCentreX();
	float needleEndY = (radius + 3 * _lineWidth) * sin(_rotation) + bounds.getBottom() - 2 * f.getHeight();

	needle.startNewSubPath(bounds.getCentreX(), bounds.getBottom() - 2 * f.getHeight());
	needle.lineTo(needleEndX, needleEndY);

	g.setColour(Colours::red);
	g.strokePath(needle, PathStrokeType(_lineWidth / 2.0f, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::rounded));

	//Make the meter take up space before drawing the labels
	bounds.removeFromTop(bounds.getHeight() - 2 * f.getHeight());
	
	g.setColour(Colours::black);
	g.drawFittedText(std::to_string(_data), bounds.removeFromTop(bounds.getHeight() / 2), Justification::centred, 1);
	g.drawFittedText(_name, bounds, Justification::centred, 1);




}


/**
 * Sets the range of data points in the speedometer.
 *
 * @param min The minimum speedometer value.
 * @param max The maximum speedometer value.
 * @throws std::out_of_range
 */
void Speedometer::setDataRange(float min, float max) {
	_dataMin = min;
	_dataMax = max;
	//Recalculate the new rotation for the data point
	setData(_data);
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

	const MessageManagerLock lck;
	repaint();
	
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
	_name = name;
	repaint();
}