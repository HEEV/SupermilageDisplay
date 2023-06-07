#include <stdexcept>

#include "Profiler/Profiler.h"
#include "Display/BurnLight.h"


BurnLight::BurnLight()
{
	FUNCTION_PROFILE();
	
	_lightL.setColor(Colours::red);
	_lightM.setColor(Colours::yellow);
	_lightR.setColor(Colours::green);

	_lightL.setName("Ready");
	_lightM.setName("Start");
	_lightR.setName("Burn");

	addAndMakeVisible(_lightL);
	addAndMakeVisible(_lightM);
	addAndMakeVisible(_lightR);
}

BurnLight::~BurnLight()
{
	FUNCTION_PROFILE();
}

void BurnLight::paint(juce::Graphics& g) 
{
	FUNCTION_PROFILE();

}

void BurnLight::resized()
{
	FUNCTION_PROFILE();

	auto bounds = getLocalBounds();
	float boundsThird = bounds.getWidth() / 3;

	_lightL.setBounds(bounds.getX(), bounds.getY(), boundsThird, bounds.getHeight());
	_lightM.setBounds(bounds.getX() + boundsThird, bounds.getY(), boundsThird, bounds.getHeight());
	_lightR.setBounds(bounds.getX() + 2*boundsThird, bounds.getY(), boundsThird, bounds.getHeight());
	

}

void BurnLight::toggleLeft() 
{
	_lightL.toggle();
}

void BurnLight::toggleMid() {
	_lightM.toggle();
}

void BurnLight::toggleRight() 
{
	_lightR.toggle();
}

void BurnLight::burn(int burnTime)
{
	int readyTime = 1000;
	int startTime = 1000;
	
	_lightL.toggle();
	Timer::callAfterDelay(readyTime, [this]() { toggleMid(); });
	Timer::callAfterDelay(readyTime + startTime, [this]() { toggleRight(); });

	Timer::callAfterDelay(burnTime + 2000, [this]() { toggleRight(); });
	Timer::callAfterDelay(burnTime + 3000, [this]() { toggleMid(); });
	Timer::callAfterDelay(burnTime + 4000, [this]() { toggleLeft(); });


}