#pragma once
#include <JuceHeader.h>
#include "ColorLight.h"

/**
* A series of three lights in a row
* 
*/

class TripleLight : public Component 
{
public:
	TripleLight();
	~TripleLight();

	// # JUCE METHODS

	void paint(juce::Graphics& g) override;
	void resized() override;

	void toggleLeft();
	void toggleMid();
	void toggleRight();

private:
	ColorLight _lightL;
	ColorLight _lightM;
	ColorLight _lightR;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TripleLight)
};