cmake_minimum_required(VERSION 3.15)

target_compile_features(SuperMileageDisplay PRIVATE cxx_std_20)
target_sources(SuperMileageDisplay
    PRIVATE
        Main.cpp
        MainComponent.cpp
        MainComponent.h
        Speedometer.cpp
        Speedometer.h
        ColorLight.cpp
        ColorLight.h
        Constants.h
        RaceTimer.h
        RaceTimer.cpp
        LapCounter.h
        LapCounter.cpp
        Formatter.h
        VerticleGauge.h
        VerticleGauge.cpp
        MapComponent.h
        MapComponent.cpp
)
