#include "ColorLight.h"

#include <stdexcept>

ColorLight::~ColorLight() {

}

/**
 * Draws the light within the graphics context.
 *
 * @param g The JUCE graphics context.
 */
void ColorLight::paint(juce::Graphics& g) 
{
	auto bounds = getLocalBounds();

	g.setColour(juce::Colours::yellow);
	g.drawRect(bounds.getCentreX() - 100, bounds.getCentreY() - 80, 200, 130);

	g.setColour(color);
	juce::Rectangle<float> house(bounds.getCentreX() - 40, bounds.getCentreY() - 70, 80, 80);
	g.fillEllipse(house);

	juce::Font theFont("Consolas", 20.0f, juce::Font::bold);
	g.setFont(theFont);
	g.setColour(juce::Colours::yellow);
	g.drawText(name, bounds.getCentreX() - 100, bounds.getCentreY() + 10, 200, 20, juce::Justification::centred, false);
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