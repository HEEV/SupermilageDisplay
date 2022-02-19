#include "Stopwatch.h"

Stopwatch::Stopwatch(juce::Point<int> center, std::string name) {
	this->center = center;
	this->name = name;
	this->startTime = std::chrono::steady_clock::now();
}

Stopwatch::Stopwatch()

{
}

Stopwatch::~Stopwatch() {

}

/**
 * Draws the light within the graphics context.
 *
 * @param g The JUCE graphics context.
 */
void Stopwatch::draw(juce::Graphics& g) {
	g.setColour(juce::Colours::yellow);
	g.drawRect(center.x - 100, center.y - 80, 200, 130);

	juce::Font theFont("Consolas", 20.0f, juce::Font::bold);
	g.setFont(theFont);
	g.drawText(name, center.x - 100, center.y + 10, 200, 20, juce::Justification::centred, false);

	juce::Font theClockFont("Consolas", 30.0f, juce::Font::bold);
	g.setFont(theClockFont);
	g.setColour(juce::Colours::white);
	g.drawText(std::to_string(minutes) + ":" + extraZero + std::to_string(seconds), center.x - 100, center.y - 30, 200, 20, juce::Justification::centred, false);
}

void Stopwatch::updateTimeSinceStartup() {
	this->currentTime = std::chrono::steady_clock::now();

	minutes = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime).count();
	seconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() - (minutes * 60);

	if (seconds < 10) extraZero = "0";
	else              extraZero = "";
}

void Stopwatch::resetClock() {
	this->startTime = std::chrono::steady_clock::now();
	minutes = 0;
	seconds = 0;
}

void Stopwatch::setName(std::string name) {
	this->name = name;
}