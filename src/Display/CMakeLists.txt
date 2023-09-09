set(
    DisplaySources
    ColorLight.cpp
    LapCounter.cpp
    MainComponent.cpp
    Main.cpp
    MapComponent.cpp
    RaceTimer.cpp
    Speedometer.cpp
    Tiltmeter.cpp
    VerticleGauge.cpp
    VerticleGaugeMir.cpp
    TripleLight.cpp
    BurnLight.cpp
    ClickLayer.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${DisplaySources})
target_sources(TestApp PRIVATE ${DisplaySources})