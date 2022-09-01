/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "Profiler.h"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 600;

//==============================================================================
class JuceFirstApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    JuceFirstApplication() {}
    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        FUNCTION_PROFILE();
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        FUNCTION_PROFILE();
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        FUNCTION_PROFILE();
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override {  /*Do nothing*/ }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              Colours::grey,
                              DocumentWindow::closeButton)
        {
            FUNCTION_PROFILE();
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);

            auto& llf = getLookAndFeel();
            llf.setColour(DocumentWindow::backgroundColourId, getBackgroundColour());
            llf.setColour(ColourIds::textColourId, Colours::black);

            //Forces GUI to be fullscreen in the car, but remain windowed for development
#ifdef DEBUG
            setResizable(true, true);
            centreWithSize (WIDTH, HEIGHT);
#else
            setResizable(false, false);
            Desktop::getInstance().setKioskModeComponent(this, false);
#endif

            getContentComponent()->setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

            Desktop::getInstance().setScreenSaverEnabled(false);

            setVisible (true);  
        }

        void closeButtonPressed() override
        {
            FUNCTION_PROFILE();
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (JuceFirstApplication)
