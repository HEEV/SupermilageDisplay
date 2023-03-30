#include "Display/TripleLight.h"

#include <stdexcept>

#include "Profiler/Profiler.h"

TripleLight::TripleLight()
{
	FUNCTION_PROFILE();

	_lightL.setColor(Colours::red);
	_lightM.setColor(Colours::red);
	_lightR.setColor(Colours::red);

	_lightL.setName("Left");
	_lightM.setName("Mid");
	_lightR.setName("Right");

	addAndMakeVisible(_lightL);
	addAndMakeVisible(_lightM);
	addAndMakeVisible(_lightR);
}

TripleLight::~TripleLight()
{
	FUNCTION_PROFILE();
}

void TripleLight::paint(juce::Graphics& g) 
{
	FUNCTION_PROFILE();


}

void TripleLight::resized()
{
	FUNCTION_PROFILE();

	auto bounds = getLocalBounds();

	int boundsThird = bounds.getWidth() / 3;

	_lightL.setBounds(bounds.getX(), bounds.getY(), boundsThird, bounds.getHeight());
	_lightM.setBounds(bounds.getX() + boundsThird, bounds.getY(), boundsThird, bounds.getHeight());
	_lightR.setBounds(bounds.getX() + 2*boundsThird, bounds.getY(), boundsThird, bounds.getHeight());
	

}

void TripleLight::toggleLeft()
{
	_lightL.toggle();
}

void TripleLight::toggleMid()
{
	_lightM.toggle();
}

void TripleLight::toggleRight()
{
	_lightR.toggle();
}