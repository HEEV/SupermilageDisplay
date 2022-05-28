#include "VerticleGauge.h"

#include "Profiler.h"

VerticleGauge::VerticleGauge(float dataMin, float dataMax) : 
	_dataMin(dataMin), _dataMax(dataMax), _data(dataMin), _prog(0.0), _bar(_prog)
{
	FUNCTION_PROFILE();
	addAndMakeVisible(_bar);
	_bar.setLookAndFeel(&_lf);
}

void VerticleGauge::paint(juce::Graphics& g)
{
	FUNCTION_PROFILE();
	g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void VerticleGauge::resized()
{
	FUNCTION_PROFILE();
	auto bounds = getLocalBounds();
	bounds.removeFromTop(10);
	bounds.removeFromBottom(10);
	_bar.centreWithSize(bounds.getWidth(), bounds.getHeight());
	
}

void VerticleGauge::update()
{
	FUNCTION_PROFILE();
}
