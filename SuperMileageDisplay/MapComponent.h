#include <JuceHeader.h>
#include <memory>

class MapComponent : public juce::AnimatedAppComponent
{
public:
	MapComponent(String trackFilepath, float mapLength);
	~MapComponent() override;

	void paint(juce::Graphics& g) override;
	void resized() override;
	void update() override;

	void updateDistance(float dist);

private:
	Path _track;
	float _trackLength;
	Point<float> _trackerPos;
	PathStrokeType _stroke;

};
