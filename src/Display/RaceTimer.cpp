#include "Display/RaceTimer.h"
#include <sstream>
#include <iomanip>

#include "Profiler/Profiler.h"

RaceTimer::RaceTimer()
{
	FUNCTION_PROFILE();
	_timeElapsed = 0;
	setFramesPerSecond(2);
}

void RaceTimer::paint(Graphics& g)
{
	FUNCTION_PROFILE();
	auto bounds = getLocalBounds();
	g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

	Font f("Consolas", 60.0f, Font::bold);
	g.setFont(f);
	g.setColour(getLookAndFeel().findColour(DocumentWindow::ColourIds::textColourId));

	short min = ((_timeElapsed / 1000) / 60) % 60;
	short sec = (_timeElapsed / 1000) % 60;
	short hour = (_timeElapsed / 1000) / 3600;

	std::stringstream ss;
	ss << hour << ((min < 10) ? ":0" : ":") << min << ":" << ((sec < 10) ? "0" : "") << sec;
	g.drawText(ss.str(), bounds, Justification::centredTop);
}

void RaceTimer::update()
{
	FUNCTION_PROFILE();
	_timeElapsed += getMillisecondsSinceLastUpdate();
}

void RaceTimer::reset() {
	_timeElapsed = 0;
}
