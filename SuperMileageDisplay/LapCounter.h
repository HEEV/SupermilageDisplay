#pragma once
#include <JuceHeader.h>

class LapCounter : public Component
{
public:
	LapCounter(double lapDistance);

	void paint(Graphics& g) override;
	void resized() override;
	void incrementLapCount();
	void setLapDist(double lapDist);

private:
	ProgressBar _lapCounter;
	ProgressBar _lapProgress;

	double _lapCount;
	double _lapDist;

	double _totalLapDistance;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LapCounter)

};
