#pragma once
#include <JuceHeader.h>

class Tiltmeter : public Component
{
public:
	Tiltmeter(float tiltLimit);
	~Tiltmeter() override;

	void paint(Graphics& g) override;
	void resized() override;

	void setCurrentTilt(float tilt);

private:
	float _tiltLimit;
	float _curTilt;
	Point<float> _guagePos;

	// Data used for drawing
	Rectangle<float> _travelLine;
	String _labelText;
	Font _font;
	Rectangle<float> _leftArea;
	Rectangle<float> _rightArea;
	ColourGradient _grad;

};
