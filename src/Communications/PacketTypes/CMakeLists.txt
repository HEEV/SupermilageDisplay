set(
    PacketTypes
    velocity.cxx
    velocityPubSubTypes.cxx
    header.cxx
    headerPubSubTypes.cxx
)

target_sources(SuperMileageDisplay PRIVATE ${PacketTypes})
target_sources(TestApp PRIVATE ${PacketTypes})
