set(
    unitsSources
    Length.cpp
)

target_sources(SuperMileageDisplay PRIVATE ${unitsSources})
target_sources(TestApp PRIVATE ${unitsSources})
