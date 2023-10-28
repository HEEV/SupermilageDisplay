#include "Display/WindDir.h"
#include "Display/Constants.h"
#include "Profiler/Profiler.h"

WindDir::WindDir(std::string name, float arrowWidth, float arrowLength) : _name(name), _arrowWidth(arrowWidth), _arrowLength(arrowLength)
{
	setData(0);
	setFramesPerSecond(30);
}

WindDir::~WindDir() {

}

//paints the arrow
void WindDir::paint(juce::Graphics& g) {
    //finds the constraints of the widget
   	auto bounds = getLocalBounds();
    float diameter = bounds.getWidth() - LINE_WEIGHT;
   	Point start = bounds.getTopLeft().toFloat();
    Point boundsCentre((float) bounds.getCentreX(), (float) bounds.getCentreY());
    constexpr float baseWidth = 20.0f;
	float length = 9.0f * diameter / 16.0f;
    PathStrokeType stroke(LINE_WEIGHT);

    //draw arc
    Path arc;
	//Since JUCE widgets start from the top-left, all custom drawing should start there for consistency
	start.addXY(LINE_WEIGHT / 2.0f, LINE_WEIGHT / 2.0f);
    arc.addArc(start.x, start.y, diameter, diameter, 0, 2 * PI, true);
	g.strokePath(arc, stroke);

    //defining arrow points
	Path hand;
	Point center(start.x + (diameter / 2.0f), start.y + (diameter / 2.0f));
	Point top((float) boundsCentre.x, boundsCentre.y - length);
	Point bottomLeft = boundsCentre - Point(baseWidth / 2.0f, -baseWidth / 2.0f);
	Point bottomRight = boundsCentre + Point(baseWidth / 2.0f, baseWidth / 2.0f);
    //drawing arrow
	hand.startNewSubPath(center);
	hand.lineTo(bottomLeft);
	hand.lineTo(top);
	hand.lineTo(bottomRight);
	hand.closeSubPath();
	hand.applyTransform(AffineTransform::rotation(_rotation, center.x, center.y));
	g.fillPath(hand);
	g.setColour(Colours::red);
}

void WindDir::setData(float value) {
    value = std::min(0.0f, std::max(2 * PI, value));

    _data = value;

    _rotation = _data;
}

void WindDir::setName(std::string name) {
    _name = name;
}
