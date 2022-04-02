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
	g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

	Font f("Consolas", 60.0f, Font::bold);
	g.setFont(f);
	g.setColour(getLookAndFeel().findColour(DocumentWindow::ColourIds::textColourId));

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
