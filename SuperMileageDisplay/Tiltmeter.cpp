#include "Tiltmeter.h"

Tiltmeter::Tiltmeter(float tiltLimit) : _tiltLimit(tiltLimit), _curTilt(0.0f)
{
}

Tiltmeter::~Tiltmeter()
{
}

void Tiltmeter::paint(Graphics& g)
{
	auto bounds = getLocalBounds();
	auto center = bounds.getCentre().toFloat();
}

void Tiltmeter::resized()
{
}

void Tiltmeter::update()
{
}

float Tiltmeter::setCurrentTilt(float tilt)
{
	return 0.0f;
}
