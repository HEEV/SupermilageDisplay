#include "Display/VerticleGaugeMir.h"
#include <fmt/format.h>
#include <utility>

#include "Profiler/Profiler.h"

VerticleGaugeMir::VerticleGaugeMir(float dataMin, float dataMax, int step, char label, float safe) : 
	_dataMin(dataMin), _dataMax(dataMax), _step(step), _data(dataMin), _label(label), _dataSafe(safe)
{
	FUNCTION_PROFILE();
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
		std::string temp = fmt::format("{:L}", (int)std::round(_dataMin + i * stepSize));
		width = std::max(width, font.getStringWidth(temp));
	}

	g.setColour(mainBC.darker());

	bounds.removeFromLeft(bounds.getWidth() / 2);
	bounds.removeFromLeft(lineOverlap);
	bounds.removeFromRight(lineOverlap);
	bounds.removeFromBottom(height);
	bounds.removeFromTop(height);
	g.fillRoundedRectangle(bounds.toFloat(), 5.0f);
	
	float redVal = ((_data - _dataMin) / (_dataSafe - _dataMin)) * (255.0f - 25.0f);
	g.setColour(Colour(redVal, 0, 0));
	int fillHeight = (_data - _dataMin) / (_dataMax - _dataMin) * bounds.getHeight();
	g.fillRoundedRectangle(bounds.getX(), bounds.getBottom() - fillHeight, bounds.getWidth(), fillHeight, 5.0f);

	
	g.setColour(lf.findColour(DocumentWindow::ColourIds::textColourId));
	int xPos = bounds.getWidth() + 2 * lineOverlap;
	for (unsigned i = 0; i <= _step; i++)
	{
		int yPos = bounds.getHeight() - bounds.getHeight() / _step * i;
		std::string label = fmt::format("{:L}", (int)std::round(_dataMin + i * stepSize));
		g.drawText(label, 0, yPos + height / 2, width, height, Justification::centredRight, false);
		g.drawLine(bounds.getX() - 2.0f, yPos + height, bounds.getRight() + 2.0f, yPos + height);
	}
	/**
	float safeHeight = bounds.getHeight() * ((_dataMax - _dataSafe) / (_dataMax - _dataMin)) + 20.0f;
	g.drawLine(xPos, safeHeight, bounds.getRight() + 2.0f, safeHeight, 5.0f);
	/**/
	Font f("Consolas", height * 2, juce::Font::bold);
	g.setFont(f);
	std::string label = fmt::format("{:L}", _label);
	g.drawText(label, bounds.getX(), bounds.getBottom() - height - 5.0f, bounds.getWidth(), height, Justification::centred, false);
	
}

void VerticleGaugeMir::resized()
{
	FUNCTION_PROFILE();	
}

void VerticleGaugeMir::update()
{
	FUNCTION_PROFILE();
}
