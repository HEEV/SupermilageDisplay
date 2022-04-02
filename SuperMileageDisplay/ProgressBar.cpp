#include "ProgressBar.h"

#define INIT_VALUE 0

ProgressBar::ProgressBar(juce::Point<float> center, float width, float height, unsigned int subDivisions, juce::Colour emptyColor, juce::Colour fillColor):
	_center(center), _width(width), _height(height), _subDivisions(subDivisions), _emptycolor(emptyColor), _fillColor(fillColor), _value(INIT_VALUE) {

}

float ProgressBar::percentFilled() const
{
	return ((float)_value) / _subDivisions;
}

void ProgressBar::draw(juce::Graphics& g)
{
	//	1. Draw the initial rectangle on which to draw the bar.
	//		a. Fill with the "empty" color.
	//		b. Draw the circumscribing rectangle.
	//	2. Draw the subdivisions.
	//		a. FOR every segment...
	//			1. IF to be filled
	//				a. Fill with "fill" color.
	//			2. ELSE
	//				b. Fill with "empty" color.
	//			3. Draw circumscribing rectangle.

	//	1. Draw the initial rectangle on which to draw the bar.
	//		a. Fill with the "empty" color.
	
	g.setColour(_emptyColor);
	juce::Rectangle<float> outsideRect(_center.x - _width / 2, _center.y - _height / 2, _width, _height);
	g.fillEllipse(outisdeRect);

	//		b. Draw the circumscribing rectangle.

	g.setColour(juce::Colours::black);
	g.drawRect(outsideRect);

	//	2. Draw the subdivisions.
	//		a. FOR every segment...

	float dWidth = _width / _subDivisions;
	float loc = _center.x - _width / 2;

	for (unsigned int i = 0; i < _subDivisions; i++) {
		juce::Rectangle<float> segment(loc, _center.y - _height / 2, dWidth, _height);

		//			1. IF to be filled
		//				a. Fill with "fill" color.
		//			2. ELSE
		//				b. Fill with "empty" color.

		if (i <= _value) {
			g.setColour(_fillColor);
		}
		else {
			g.setColour(_emptyColor);
		}
		g.fillEllipse(segment);

		//			3. Draw circumscribing rectangle.

		g.setColour(juce::Colours::black);
		g.drawRect(segment);
	}
}

unsigned int ProgressBar::getValue() const {
	return _value;
}

void ProgressBar::setValue(unsigned int x) {
	_value = x;
}

unsigned int ProgressBar::getSubDivisions() const
{
	return _subDivisions;
}
