#include <JuceHeader.h>

constexpr float ARROW_LENGTH = 40.0f;
class MapComponent : public juce::Component
{
public:
	MapComponent(String trackFilepath, float mapLength);
	~MapComponent() override;

	void paint(juce::Graphics& g) override;
	void resized() override;

	void updateDistance(float dist);
	void incDistance(float deltaDist);

private:
	Path _track;
	float _trackLength;
	Point<float> _trackerPos;
	Point<float> _lastPos;
	PathStrokeType _stroke;
	float _distanceAlong;

};
