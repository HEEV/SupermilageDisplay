#include "Display/VerticleGaugeMir.h"
#include <fmt/format.h>
#include <utility>

#include "Profiler/Profiler.h"

VerticleGaugeMir::VerticleGaugeMir(float dataMin, float dataMax, int step) : 
	_dataMin(dataMin), _dataMax(dataMax), _step(step), _data(dataMin)
{	
	FUNCTION_PROFILE();
	setFramesPerSecond(30);
}

void VerticleGaugeMir::paint(juce::Graphics& g)
{
	FUNCTION_PROFILE();
	constexpr unsigned lineOverlap = 2;

	auto& lf = getLookAndFeel();
	Colour mainBC = lf.findColour(DocumentWindow::backgroundColourId);
	g.fillAll(mainBC);
	auto bounds = getLocalBounds();
	auto font = g.getCurrentFont();
	int width = 0.0f;
	int height = font.getHeight();

	float stepSize = (_dataMax - _dataMin) / _step;
	for (unsigned i = 0; i <= _step; i++)
	{
		std::string temp = fmt::format("%d", _dataMin + i * stepSize);
		width = std::max(width, font.getStringWidth(temp));
	}

	g.setColour(mainBC.darker());

	bounds.removeFromLeft(width);
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
		std::string label = fmt::format("{:L}", (int)std::round(_dataMin + i * stepSize));
		width = font.getStringWidth(label);
		g.drawText(label, 0, yPos + height / 2, width, height, Justification::centredRight, false);
		g.drawLine(width, yPos + height, xPos + width, yPos + height);
	}
	
}

void VerticleGaugeMir::resized()
{
	FUNCTION_PROFILE();	
}

void VerticleGaugeMir::update()
{
	FUNCTION_PROFILE();
}
