#include "SizeableSpeedometer.h"

SizeableSpeedometer::SizeableSpeedometer(Vertices vertices, float min, float max, std::string name, juce::Colour color, int readoutOffset) :
	_vertices(vertices), _dataMin(min), _dataMax(max), _name(name), _color(color), _readoutOffset(readoutOffset) {
	
}

void SizeableSpeedometer::draw(juce::Graphics& g) {
	g.setColour(_color);

}