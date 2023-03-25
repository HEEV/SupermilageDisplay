/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <locale>
#include "Display/MainComponent.h"
#include "Profiler/Profiler.h"

#ifdef TESTING_ENABLED
#include <gtest/gtest.h>
#include <thread>
#include <future>
#include <memory>
#endif

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 600;

//==============================================================================
class SuperMileageApp  : public juce::JUCEApplication
{
public:
    //==============================================================================
    SuperMileageApp() {}
    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return false; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        FUNCTION_PROFILE();
        mainWindow.reset (new MainWindow (getApplicationName()));

#ifdef TESTING_ENABLED
        auto params = juce::JUCEApplicationBase::getCommandLineParameterArray();

        //Convert JUCE types to c primitives for gtest
        //Note that params has the name of the app stripped, which gtest is expecting
        int argc = params.size() + 1;
        const std::string& name = getApplicationName().toStdString();
        char** args = new char*[argc];

        args[0] = new char[name.size()];
        std::strcpy(args[0], name.c_str());
        for(unsigned i = 1; i < argc; i++)
        {
            const juce::String& ref = params.strings[i];
            args[i] = new char[ref.length()];
            std::strcpy(args[i], ref.toStdString().c_str());
        }
        ::testing::InitGoogleTest(&argc, args);
        _testResult = std::async([&]()
            {
                std::cout << "Starting Tests\n";
                int retValue = RUN_ALL_TESTS();
                std::cout << "Tests completed. Quitting...\n";
                systemRequestedQuit();
                std::cout << "JUCE quit event posted\n";
                return retValue;
            }
        );
#endif
        
    }

    void shutdown() override
    {
        FUNCTION_PROFILE();
        mainWindow = nullptr;
        #ifdef TESTING_ENABLED
        int testResult = 0;
        if(_testResult.valid())
        {
            testResult = _testResult.get();
            std::cout << "GTest succesfully finished\n";
        }
        juce::JUCEApplicationBase::setApplicationReturnValue(testResult);
        #endif
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
                              Colours::deepskyblue,
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

            //Set locale for our formatting lib
            std::locale::global(std::locale("en_US.UTF-8"));

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
#ifdef TESTING_ENABLED
    std::future<int> _testResult;
#endif
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (SuperMileageApp)
