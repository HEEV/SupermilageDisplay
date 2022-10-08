#pragma once
#include <JuceHeader.h>

class Tiltmeter : public AnimatedAppComponent
{
public:
	Tiltmeter(float tiltLimit);
	~Tiltmeter() override;

	void paint(Graphics& g);
	void resized() override;
	void update() override;

	float setCurrentTilt(float tilt);

private:
	float _tiltLimit;
	float _curTilt;

};
