#pragma once

#include <vector>
#include <JuceHeader.h>

/*
* Interface for items that can be sized with the Sizer class.
*/
class ISizeable {
	virtual std::vector<juce::Point<int>*> exposeVertices() = 0;
};