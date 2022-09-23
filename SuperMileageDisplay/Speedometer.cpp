#include <stdexcept>
#include <sstream>
#include <locale>

#include "Profiler.h"
#include "Speedometer.h"
#include "Constants.h"
#include "Formatter.h"

Speedometer::Speedometer(std::string_view name, float minData, float maxData, juce::Colour color, int subdivisions, int lineWidth) :
	_name(name), _dataMin(minData), _dataMax(maxData), 
	_color(color), _lineWidth(lineWidth), _subdivisions(subdivisions), _lc(nullptr)
{
	FUNCTION_PROFILE();
	setData(_dataMin);
	setFramesPerSecond(30);
}

/**
 * Destructor.
 */
Speedometer::~Speedometer() {
	FUNCTION_PROFILE();

}

/**
 * Draws the speedometer within the graphics context.
 * Do NOT call - called by JUCE
 *
 * @param g The JUCE graphics context.
 */
void Speedometer::paint(juce::Graphics& g) 
{
	auto bounds = getLocalBounds();
	constexpr float FONT_HEIGHT = 20.0f;
	g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
	Font f("Consolas", FONT_HEIGHT, juce::Font::bold);
	g.setFont(f);

	Rectangle<int> gaugeArea;
	if (bounds.getWidth() < bounds.getHeight())
		gaugeArea = bounds;
	else

}

void Speedometer::update()
{
	if (_lc)
		_lc->incDistanceTraveled((1.0f / 30.0f) / 3600.0f *_data);
}

void Speedometer::addLapCounter(LapCounter* lc)
{
	_lc = lc;
}


/**
 * Sets the range of data points in the speedometer.
 *
 * @param min The minimum speedometer value.
 * @param max The maximum speedometer value.
 * @throws std::out_of_range
 */
void Speedometer::setDataRange(float min, float max) {
	FUNCTION_PROFILE();
	_dataMin = min;
	_dataMax = max;
	//Recalculate the new rotation for the data point
	setData(_data);
}

/**
 * Sets the speedometer value.
 *
 * @param The speedometer value.
 */
void Speedometer::setData(float value) {
	FUNCTION_PROFILE();

	value = std::min(_dataMax, std::max(_dataMin, value));

	_data = value;

	// Range the data around 0
	float weight = _dataMax - _dataMin;
	value = value - _dataMin;

	// Set the rotation
	_rotation = PI * (weight + value) / weight;
	
}

/**
 * Gets the data that is displaying on the speedometer.
 *
 * @return The speedometer data.
 */
float Speedometer::getData() const {
	FUNCTION_PROFILE();
	return _data;
}

/*
 * Sets the name of the speedometer.
 *
 * Params:
 * name -> The new name the speedometer.
 */
void Speedometer::setName(std::string name) {
	FUNCTION_PROFILE();
	_name = name;
}
