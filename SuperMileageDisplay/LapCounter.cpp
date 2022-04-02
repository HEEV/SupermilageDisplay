#include "LapCounter.h"

LapCounter::LapCounter(double lapDistance) :
	_lapCounter(_lapCount), _lapProgress(_lapDist), _lapCount(0), _lapDist(0), _totalLapDistance(lapDistance)
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

void LapCounter::incrementLapCount() {
	if (_lapCount < 0.68) {
		_lapCount += (0.3333333);
		_lapCounter.setTextToDisplay("Lap " + std::to_string((int)(_lapCount * 3) + 2));
		if (_lapCount >= 0.68) {
			_lapCounter.setTextToDisplay("Laps Complete");
		}
	}
}

void LapCounter::setLapDist(double lapDist) {
	_lapDist = lapDist;
	if (_lapDist >= 1.0) {
		_lapDist = 0;
		incrementLapCount();
	}
}