#pragma once
#include <JuceHeader.h>

class WindDir : public AnimatedAppComponent
{
    public:
        WindDir(std::string name, float arrowWidth, float arrowLength);
        ~WindDir();

        //JUCE methods

        void paint(juce::Graphics& g) override;
	    void resized() override { /*do nothing*/ }
	    void update() override { /* do nothing */}

        void setData(float data);
        void setName(std::string name);
    
    private:
        float _rotation;
        float _data;
        float _arrowWidth;
        float _arrowLength;
        std::string _name;
        juce::Colour _color;
};