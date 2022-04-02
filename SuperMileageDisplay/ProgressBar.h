//
// A progress bar JUCE widget, upon which to display lap data
// in the car.
//
// Author:	Ian Hubble, Andrew Huffman
// Date:	November 13, 2021
//

#include <JuceHeader.h>

class ProgressBar {
public:

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="center">The center-point of the widget.</param>
	/// <param name="subDivisions">The amount of subdivisions on the progress bar.</param>
	/// <param name="emptyColor">The color which "empty" bar subdivisions take on.</param>
	/// <param name="fillColor">The color which "filled" bar subdivisions take on.</param>
	ProgressBar(juce::Point<float> center, float width, float height, unsigned int subDivisions, juce::Colour emptyColor, juce::Colour fillColor);

	float percentFilled() const;

	void draw(juce::Graphics& g);

	unsigned int getValue() const;
	void setValue(unsigned int x);

	unsigned int getSubDivisions() const;

private:
	juce::Point<float> _center;
	float _width;
	float _height;
	unsigned int _subDivisions;
	juce::Colour _emptyColor;
	juce::Colour _fillColor;

	unsigned int _value;
};