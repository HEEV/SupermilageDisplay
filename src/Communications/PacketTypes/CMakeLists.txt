set(
    PacketTypes
    velocity.cxx
    velocityPubSubTypes.cxx
)

target_sources(SuperMileageDisplay PRIVATE ${PacketTypes})
target_sources(TestApp PRIVATE ${PacketTypes})
