set(
    PacketTypes
    velocity.cxx
    velocityPubSubTypes.cxx
    velocityTypeObject.cxx
    header.cxx
    headerPubSubTypes.cxx
    headerTypeObject.cxx
)

target_sources(SuperMileageDisplay PRIVATE ${PacketTypes})
target_sources(TestApp PRIVATE ${PacketTypes})
