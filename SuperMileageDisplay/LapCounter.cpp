#include "LapCounter.h"

LapCounter::LapCounter(double lapLength, unsigned lapAmount) :
	_lapCounter(_lapCount), _lapProgress(_lapDist), _lapCount(0), _lapDist(0), _lapLength(lapLength), _lapAmount(lapAmount)
{
	_lapCounter.setTextToDisplay("Lap 1");

	addAndMakeVisible(_lapCounter);
	addAndMakeVisible(_lapProgress);
}

void LapCounter::paint(Graphics& g)
{
}

void LapCounter::resized()
{
	auto bounds = getLocalBounds();
	constexpr int margin = 5;

	bounds.removeFromTop(margin);
	bounds.removeFromLeft(margin);
	bounds.removeFromRight(margin);

	_lapCounter.setBounds(bounds.removeFromTop(bounds.getHeight() / 2 - margin));
	bounds.removeFromTop(2 * margin);
	bounds.removeFromBottom(margin);
	_lapProgress.setBounds(bounds);
}

void LapCounter::incDistanceTraveled(double dist)
{
	_lapDist += dist / _lapLength;
	if (_lapDist >= 1.0)
	{
		_lapCount += 1.0 / _lapAmount;

		_lapDist = 0.0;

		const MessageManagerLock lck;
		_lapCounter.setTextToDisplay("Lap " + std::to_string((int)_lapCount));
	}
}
