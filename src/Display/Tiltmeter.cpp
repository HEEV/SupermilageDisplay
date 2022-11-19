#include "Display/Tiltmeter.h"
#include "Display/Constants.h"

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
	g.setFont(_font);

	//g.setGradientFill(_grad);
	//g.fillRect(_travelLine);
	g.drawEllipse(_travelLine, 1);
	g.setColour(Colours::black);

	g.drawText("-" + _labelText, _leftArea, Justification::centred);
	g.drawText(_labelText, _rightArea, Justification::centred);
	g.setColour(Colours::red);
	g.fillEllipse(_guagePos.x - LINE_WEIGHT / 2.0f, _guagePos.y - LINE_WEIGHT / 2.0f, LINE_WEIGHT, LINE_WEIGHT);
	g.setColour(Colours::black);

}

void Tiltmeter::resized()
{
	float labelWidth = _font.getStringWidthFloat(_labelText) * 1.5f;

	auto bounds = getLocalBounds();
	auto center = bounds.getCentre().toFloat();
	_travelLine.setWidth(bounds.getWidth() - 2 * labelWidth - 10.0f);
	//_travelLine.setHeight(LINE_WEIGHT / 4.0f);
	_travelLine.setHeight(bounds.getWidth() - 2 * labelWidth - 10.0f);
	_travelLine.setCentre(center);

	_leftArea.setRight(_travelLine.getX());
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
	_grad.addColour(0.5, Colour(0, 0, 0));

	setCurrentTilt(_curTilt);
}

void Tiltmeter::setCurrentTilt(float tilt)
{
	_curTilt = tilt;
	
	float length = _travelLine.getRight() - _travelLine.getCentreX();
	float step = length / _tiltLimit;
	_guagePos.x = tilt * step + _travelLine.getCentreX();

	_guagePos.y = _travelLine.getCentreY();
}
