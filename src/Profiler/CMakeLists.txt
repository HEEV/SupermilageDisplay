set(
    ProfilerSources
    Profiler.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${ProfilerSources})
target_sources(TestApp PRIVATE ${ProfilerSources})
