#pragma once
#include <JuceHeader.h>
#include "ColorLight.h"

/**
* A series of three lights in a row
* 
*/

class BurnLight : public Component 
{
public:
	BurnLight();
	~BurnLight();

	// # JUCE METHODS

	void paint(juce::Graphics& g) override;
	void resized() override;

	void burn(int burnTime = 1000);

private:
	ColorLight _lightL;
	ColorLight _lightM;
	ColorLight _lightR;

	void toggleLeft();
	void toggleMid();
	void toggleRight();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BurnLight)
};