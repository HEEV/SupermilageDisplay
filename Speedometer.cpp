#include "Speedometer.h"

Speedometer::Speedometer(juce::Point<int> center) {
	this->center = center;
}

Speedometer::~Speedometer() {

}

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

void Speedometer::setRotation(float newRotation) {
	this->rotation = newRotation;
}

void Speedometer::setDataRange(float min, float max) {
	this->dataMin = min;
	this->dataMax = max;
}

void Speedometer::updateDataValue(float value) {
	setRotation(3.1415 * (dataMax + value) / dataMax);
}