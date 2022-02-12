#include "ProgressBar.h"

ProgressBar::ProgressBar(juce::Point<int> center, std::string name, unsigned int currentVal, juce::Colour fillColor, juce::Colour emptyColor, unsigned int maxVal) {
	_center = center;
	_max_val = maxVal;
	_val = currentVal;
	_on_full = nullptr;
	_name = name;
	_fill_color = fillColor;
	_empty_color = emptyColor;
}

ProgressBar::ProgressBar(juce::Point<int> center, std::string name, unsigned int currentVal):
	ProgressBar(center, name, currentVal, juce::Colour(0xff, 0x00,  0x00), juce::Colour(0x00, 0xff, 0x00),  10) {
}

ProgressBar::~ProgressBar() {
	// I. Draw the outer rectangle for the progress bar about
	// the center.
	// II. Fill the progress bar proportional to the amount 
	// that has been completed.
}

void ProgressBar::draw(juce::Graphics& g) {

}

void ProgressBar::setMaxValue(unsigned int maxVal) {
	_max_val = maxVal;

	_checkFull();
}

void ProgressBar::setValue(unsigned int val) {
	_val = val;

	_checkFull();
}

void ProgressBar::setOnFull(Callback callback) {
	_on_full = callback;
}

unsigned int ProgressBar::getMaxValue() const {
	return _max_val;
}

unsigned int ProgressBar::getValue() const {
	return _val;
}

void ProgressBar::_checkFull() {
	if (_val >= _max_val && _on_full) {
		_on_full();
	}
}