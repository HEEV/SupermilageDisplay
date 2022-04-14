#pragma once
#include <JuceHeader.h>
#include <locale>

class LapCounter : public Component
{
public:
	LapCounter(double lapLength, unsigned lapAmount);

	void paint(Graphics& g) override;
	void resized() override;

	void incDistanceTraveled(double dist);



private:
	ProgressBar _lapCounter;
	ProgressBar _lapProgress;

	double _lapCount;
	double _lapDist;

	double _lapLength;
	unsigned _lapAmount;

	struct Formatter : public std::numpunct<char>
	{
		std::string do_grouping() const override { return "\3"; }
	};

	bool _finished;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LapCounter)

};
