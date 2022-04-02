#include "RaceTimer.h"
#include <sstream>
#include <iomanip>

RaceTimer::RaceTimer()
{
	_timeElapsed = 0;
	setFramesPerSecond(2);
}

void RaceTimer::paint(Graphics& g)
{
	auto bounds = getLocalBounds();
	g.fillAll(Colours::darkgrey);

	Font f("Consolas", 40.0f, Font::bold);
	g.setFont(f);
	g.setColour(Colours::red);

	short min = (_timeElapsed / 1000) / 60;
	short sec = (_timeElapsed / 1000) % 60;

	std::stringstream ss;
	ss << min << ":" << ((sec < 10) ? "0" : "") << sec;
	g.drawText(ss.str(), bounds, Justification::centredTop);
}

void RaceTimer::update()
{
	_timeElapsed += getMillisecondsSinceLastUpdate();
}
