#include "ColorLight.h"

#include <stdexcept>

#include "Profiler.h"

ColorLight::ColorLight(std::string name, Colour color) : _name(name), _color(color)
{
	FUNCTION_PROFILE();
}

ColorLight::~ColorLight() {
	FUNCTION_PROFILE();
}

/**
 * Draws the light within the graphics context.
 *
 * @param g The JUCE graphics context.
 */
void ColorLight::paint(juce::Graphics& g) 
{
	FUNCTION_PROFILE();
	auto bounds = getLocalBounds();

	juce::Font f("Consolas", 20.0f, juce::Font::bold);
	g.setFont(f);

	g.setColour(_color);
	auto circle = bounds.removeFromTop(bounds.getHeight() - f.getHeight());
	circle.setWidth(circle.getHeight());
	circle.setCentre(bounds.getCentreX(), circle.getCentreY());
	g.fillEllipse(circle.toFloat());
	
	g.setColour(getLookAndFeel().findColour(DocumentWindow::ColourIds::textColourId));
	g.drawText(_name, bounds, juce::Justification::centred, false);
}

void ColorLight::setColor(juce::Colour color) {
	FUNCTION_PROFILE();
	_color = color;
	const MessageManagerLock lck;
	repaint();
}

const juce::Colour& ColorLight::getColor() const {
	FUNCTION_PROFILE();
	return _color;
}

void ColorLight::setName(const std::string& name) {
	FUNCTION_PROFILE();
	_name = name;
}