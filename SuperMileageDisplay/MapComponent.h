#include <JuceHeader.h>

class MapComponent : public juce::Component
{
public:
	MapComponent(float mapLength);
	~MapComponent() override;

	void paint(juce::Graphics& g) override;
	void resized() override;

private:
	float _mapLength;
	float _distanceAlong;

};
