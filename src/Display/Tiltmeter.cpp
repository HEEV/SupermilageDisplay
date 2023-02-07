#include "Display/Tiltmeter.h"
#include "Display/Constants.h"
#include <fmt/format.h>

Tiltmeter::Tiltmeter(float tiltLimit) : 
	_tiltLimit(tiltLimit), _curTilt(0.0f), _guagePos(0.0f, 0.0f), _travelLine(), _labelText(tiltLimit),
	_font("Consolas", FONT_HEIGHT, Font::bold), _leftArea(), _rightArea(), _grad()
{
	setCurrentTilt(0.0f);
}

Tiltmeter::~Tiltmeter()
{
}

void Tiltmeter::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
	Font f("Consolas", FONT_HEIGHT, juce::Font::bold);
	g.setFont(f);
	PathStrokeType stroke(LINE_WEIGHT / 2);

	// init arc variables
	Point<float> start = _travelLine.getTopLeft().toFloat();
	start.addXY(LINE_WEIGHT / 2.0f, LINE_WEIGHT / 2.0f);
	float diameter = _travelLine.getWidth() - LINE_WEIGHT;

	// draw circle
	g.setColour(Colour(130, 0, 0));
	g.drawEllipse(start.x, start.y, diameter, diameter, LINE_WEIGHT / 2);

	//Draw semicircle
	g.setColour(Colours::black);
	Path arc;
	//Since JUCE widgets start from the top-left, all custom drawing should start there for consistency
	arc.addArc(start.x, start.y, diameter, diameter, (PI / 2) - _tiltLimit, (PI / 2) + _tiltLimit, true);
	arc.addArc(start.x, start.y, diameter, diameter, (-PI / 2) + _tiltLimit, (-PI / 2) - _tiltLimit, true);
	g.strokePath(arc, stroke);

	// change hand color
	if ((_curTilt > _tiltLimit) || (_curTilt < -_tiltLimit)) {
		g.setColour(Colours::red);
	}

	// draw hand
	Path hand;
	Point left(_travelLine.getCentreX() - _travelLine.getWidth() / 2, _travelLine.getCentreY());
	Point right(_travelLine.getCentreX() + _travelLine.getWidth() / 2, _travelLine.getCentreY());
	Point top(_travelLine.getCentreX(), _travelLine.getCentreY() + 5);
	Point bottom(_travelLine.getCentreX(), _travelLine.getCentreY() - 5);
	hand.startNewSubPath(left);
	hand.lineTo(top);
	hand.lineTo(right);
	hand.lineTo(bottom);
	hand.closeSubPath();
	hand.applyTransform(AffineTransform::rotation(_curTilt, _travelLine.getCentreX(), _travelLine.getCentreY()));
	g.fillPath(hand);

	g.setColour(Colours::black);

	//Draw digital readout
	Point<float> labelCenter = _travelLine.getCentre();
	Rectangle<float> readoutArea;
	String readout = fmt::format("{:.1f}", _curTilt * (180/PI));
	float readoutWidth = f.getStringWidthFloat(readout);
	readoutArea.setSize(readoutWidth, FONT_HEIGHT);
	readoutArea.setCentre(labelCenter.x, labelCenter.y + FONT_HEIGHT);
	g.drawText(readout, readoutArea, Justification::centred);
}

void Tiltmeter::resized()
{
	float labelWidth = _font.getStringWidthFloat(_labelText) * 1.5f;

	auto bounds = getLocalBounds();
	auto center = bounds.getCentre().toFloat();
	//_travelLine.setWidth(bounds.getWidth() - 2 * labelWidth - 10.0f);
	_travelLine.setWidth(bounds.getHeight() - 10);
	//_travelLine.setHeight(LINE_WEIGHT / 4.0f);
	_travelLine.setHeight(bounds.getHeight() - 10);
	_travelLine.setCentre(center);

	/*_leftArea.setRight(_travelLine.getX());
	_leftArea.setWidth(labelWidth);
	_leftArea.setHeight(FONT_HEIGHT);
	_leftArea.setCentre(_leftArea.getCentreX(), _travelLine.getY());

	_rightArea.setX(_travelLine.getRight());
	_rightArea.setWidth(labelWidth);
	_rightArea.setHeight(FONT_HEIGHT);
	_rightArea.setCentre(_rightArea.getCentreX(), _travelLine.getY());

	_grad = ColourGradient::horizontal
		(
			Colour(128, 128, 128), _travelLine.getX(),
			Colour(128, 128, 128), _travelLine.getRight()
		);
	_grad.addColour(0.5, Colour(0, 0, 0));*/

	setCurrentTilt(_curTilt);
}

void Tiltmeter::setCurrentTilt(float tilt)
{
	_curTilt = tilt;

	
	/*float length = _travelLine.getRight() - _travelLine.getCentreX();
	float step = length / _tiltLimit;*/
	//_guagePos.x = tilt - (_tiltLimit / 2) + PI/2;
}
