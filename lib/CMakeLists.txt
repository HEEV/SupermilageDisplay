# Load and configure deps
set(JUCE_BUILD_EXTRAS off CACHE BOOL "Disables projucer" FORCE)
set(JUCE_BUILD_EXAMPLES off CACHE BOOL "Disables JUCE examples" FORCE)
add_subdirectory(JUCE)
add_subdirectory(fmt EXCLUDE_FROM_ALL)
include_directories(SYSTEM fmt/include)
target_compile_options(fmt PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -w>
     $<$<CXX_COMPILER_ID:MSVC>:
          /w>)

# Load GoogleTest for our unit testing
include(GoogleTest)
add_subdirectory(googletest)

include(cmake/getDDSDeps.cmake)
add_subdirectory(Fast-DDS)
