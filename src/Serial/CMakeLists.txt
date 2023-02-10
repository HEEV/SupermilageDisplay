set(
    SerialSources
    NewSerialClient.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${SerialSources})
target_sources(TestApp PRIVATE ${SerialSources})
