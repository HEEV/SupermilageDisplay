#include "ColorLight.h"

#include <stdexcept>

ColorLight::ColorLight(juce::Point<int> center, juce::Colour colorStart, std::string name) {
	this->center = center;
	this->color = colorStart;
	this->name = name;
}

ColorLight::ColorLight(juce::Point<int> center) {
	this->center = center;
	this->color = juce::Colours::white;
	this->name = "ColorLight";
}

ColorLight::~ColorLight() {

}

/**
 * Draws the light within the graphics context.
 *
 * @param g The JUCE graphics context.
 */
void ColorLight::draw(juce::Graphics& g) {
	g.setColour(juce::Colours::yellow);
	g.drawRect(center.x - 100, center.y - 80, 200, 130);

	g.setColour(color);
	juce::Rectangle<float> house(center.x - 40, center.y - 70, 80, 80);
	g.fillEllipse(house);

	juce::Font theFont("Consolas", 20.0f, juce::Font::bold);
	g.setFont(theFont);
	g.setColour(juce::Colours::yellow);
	g.drawText(name, center.x - 100, center.y + 10, 200, 20, juce::Justification::centred, false);
}

void ColorLight::setColor(juce::Colour color) {
	this->color = color;
}

juce::Colour ColorLight::getColor() const {
	return this->color;
}

void ColorLight::setName(std::string name) {
	this->name = name;
}