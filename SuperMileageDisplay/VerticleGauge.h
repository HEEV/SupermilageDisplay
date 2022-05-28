#pragma once
#include <JuceHeader.h>

class VerticleGauge : public AnimatedAppComponent
{
public:
	VerticleGauge(float dataMin, float dataMax);

	void paint(juce::Graphics& g) override;
	void resized() override;
	void update() override;

	void setData(float data) { _data = data; }

private:
	float _dataMin;
	float _dataMax;
	float _data;

	double _prog;
	LookAndFeel_V2 _lf;
	ProgressBar _bar;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticleGauge)
};
