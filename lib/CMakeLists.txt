# Load and configure deps
set(JUCE_BUILD_EXTRAS off CACHE BOOL "Disables projucer" FORCE)
set(JUCE_BUILD_EXAMPLES off CACHE BOOL "Disables JUCE examples" FORCE)
add_subdirectory(JUCE)

# Load GoogleTest for our unit testing
include(GoogleTest)
add_subdirectory(googletest)

add_subdirectory(Fast-DDS-Communication-Manager)
