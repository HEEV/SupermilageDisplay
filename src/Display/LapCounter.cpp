#include "Display/LapCounter.h"
#include <fmt/format.h>
#include <string>

#include "Profiler/Profiler.h"

LapCounter::LapCounter(double lapLength, unsigned lapAmount) :
	_lapCounter(_lapCount), _lapProgress(_lapDist), _lapCount(0), 
	_lapDist(0), _lapLength(lapLength), _lapAmount(lapAmount), _finished(false)
{
	FUNCTION_PROFILE();

	_lapCounter.setColour(_lapCounter.foregroundColourId, Colour(253, 185, 19));
	_lapProgress.setColour(_lapCounter.foregroundColourId, Colour(253, 185, 19));

	_lapCounter.setTextToDisplay("Lap 1");

	addAndMakeVisible(_lapCounter);
	addAndMakeVisible(_lapProgress);
}

void LapCounter::paint(Graphics& g)
{
	FUNCTION_PROFILE();
}

void LapCounter::resized()
{
	FUNCTION_PROFILE();

	_lapCounter.setColour(_lapCounter.foregroundColourId, Colours::cornsilk);
	_lapProgress.setColour(_lapCounter.foregroundColourId, Colours::cornsilk);

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
	FUNCTION_PROFILE();

	if (!_finished)
	{
		_lapDist += dist / _lapLength;
		std::string label = fmt::format("{:L} ft", (int)(_lapDist * 5280.0f));
		_lapProgress.setTextToDisplay(label);

		if (_lapDist >= 1.0)
		{
			_lapCount += 1.0 / _lapAmount;

			_lapDist = 0.0;

			const MessageManagerLock lck;
			if (_lapCount < 1.0f)
				_lapCounter.setTextToDisplay(fmt::format("Lap {}", (int)(_lapCount * _lapAmount + 1)));
			else
			{
				_lapCounter.setTextToDisplay("Finished");
				_lapCounter.setColour(_lapCounter.foregroundColourId, Colours::green);
				_lapProgress.setColour(_lapProgress.foregroundColourId, Colours::green);
				_lapDist = 1.0f;

				_finished = true;
			}
		}
	}
}

void LapCounter::reset()
{
	_lapCount = 0;
	_lapDist = 0;
	
	_lapCounter.setColour(_lapCounter.foregroundColourId, Colours::cornsilk);
	_lapProgress.setColour(_lapCounter.foregroundColourId, Colours::cornsilk);

	_lapCounter.setTextToDisplay("Lap 1");
}
