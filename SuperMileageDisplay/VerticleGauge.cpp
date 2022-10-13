#include "VerticleGauge.h"
#include <sstream>
#include <locale>
#include <utility>

#include "Profiler.h"
#include "Formatter.h"

VerticleGauge::VerticleGauge(float dataMin, float dataMax, int step) : 
	_dataMin(dataMin), _dataMax(dataMax), _step(step), _data(dataMin)
{
	FUNCTION_PROFILE();
}

void VerticleGauge::paint(juce::Graphics& g)
{
	FUNCTION_PROFILE();
	constexpr unsigned lineOverlap = 2;

	auto& lf = getLookAndFeel();
	Colour mainBC = lf.findColour(DocumentWindow::backgroundColourId);
	g.fillAll(mainBC);
	std::stringstream ss;
	ss.imbue(std::locale(ss.getloc(), new Formatter));
	auto bounds = getLocalBounds();
	auto font = g.getCurrentFont();
	int width = 0.0f;
	int height = font.getHeight();

	float stepSize = (_dataMax - _dataMin) / _step;
	for (unsigned i = 0; i <= _step; i++)
	{
		ss.str("");
		ss << _dataMin + i * stepSize;
		width = std::max(width, font.getStringWidth(ss.str()));
	}

	g.setColour(mainBC.darker());

	bounds.removeFromRight(width);
	bounds.removeFromLeft(lineOverlap);
	bounds.removeFromRight(lineOverlap);
	bounds.removeFromBottom(height);
	bounds.removeFromTop(height);
	g.fillRoundedRectangle(bounds.toFloat(), 5.0f);

	g.setColour(Colours::red);
	int fillHeight = (_data - _dataMin) / (_dataMax - _dataMin) * bounds.getHeight();
	g.fillRoundedRectangle(bounds.getX(), bounds.getBottom() - fillHeight, bounds.getWidth(), fillHeight, 5.0f);

	g.setColour(lf.findColour(DocumentWindow::ColourIds::textColourId));
	int xPos = bounds.getWidth() + 2 * lineOverlap;
	for (unsigned i = 0; i <= _step; i++)
	{
		int yPos = bounds.getHeight() - bounds.getHeight() / _step * i;
		ss.str("");
		float num = _dataMin + i * stepSize;
		ss << num;
		g.drawText(ss.str(), xPos, yPos + height / 2, width, height, Justification::centredLeft, false);
		g.drawLine(0, yPos + height, xPos, yPos + height);
	}
}

void VerticleGauge::resized()
{
	FUNCTION_PROFILE();	
}

void VerticleGauge::update()
{
	FUNCTION_PROFILE();
}
