#pragma once
#include <JuceHeader.h>

class RaceTimer : public AnimatedAppComponent
{
public:
	RaceTimer();

	void paint(Graphics& g) override;
	void update() override;
	void reset();


private:
	unsigned int _timeElapsed;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RaceTimer)
};
