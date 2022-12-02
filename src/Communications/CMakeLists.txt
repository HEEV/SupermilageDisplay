set(
    CommSources
    CommunicationManager.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${CommSources})
target_sources(TestApp PRIVATE ${CommSources})
