set(
    DisplaySources
    CMakeLists.txt
    ColorLight.cpp
    LapCounter.cpp
    MainComponent.cpp
    Main.cpp
    MapComponent.cpp
    RaceTimer.cpp
    Speedometer.cpp
    Tiltmeter.cpp
    VerticleGauge.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${DisplaySources})
target_sources(TestApp PRIVATE ${DisplaySources})