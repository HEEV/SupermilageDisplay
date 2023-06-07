#pragma once
#include <JuceHeader.h>

class VerticleGaugeMir : public AnimatedAppComponent
{
public:
	VerticleGaugeMir(float dataMin, float dataMax, int step, char label, float safe);

	void paint(juce::Graphics& g) override;
	void resized() override;
	void update() override;

	void setData(float data) { _data = data; }

private:
	float _dataMin;
	float _dataMax;
	float _dataSafe;
	int _step;
	float _data;
	char _label;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticleGaugeMir)
};
