#include "Communication/CommunicationManager.h"
#include <istream>
#include <chrono>
#include "Communication/PacketTypes/header.h"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastrtps::rtps;

CommunicationManager::CommunicationManager()
{
    // Get default participant QoS
    DomainParticipantQos client_qos = PARTICIPANT_QOS_DEFAULT;

    // Set participant as CLIENT
    client_qos.wire_protocol().builtin.discovery_config.discoveryProtocol =
            DiscoveryProtocol_t::CLIENT;

    // Set SERVER's GUID prefix
    RemoteServerAttributes remote_server_att;
    remote_server_att.ReadguidPrefix("44.53.00.5f.45.50.52.4f.53.49.4d.41");

    // Set SERVER's listening locator for PDP
    Locator_t locator;
    IPLocator::setIPv4(locator, 10, 13, 76, 54);
    locator.port = 25565;
    remote_server_att.metatrafficUnicastLocatorList.push_back(locator);

    // Add remote SERVER to CLIENT's list of SERVERs
    client_qos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(remote_server_att);

    // Set ping period to 250 ms
    client_qos.wire_protocol().builtin.discovery_config.discoveryServer_client_syncperiod =
            eprosima::fastrtps::Duration_t(0, 250000000);

    client_qos.wire_protocol().builtin.typelookup_config.use_client = true;
    client_qos.wire_protocol().builtin.typelookup_config.use_server = true;

    // Create CLIENT
    _particpant =
            DomainParticipantFactory::get_instance()->create_participant(0, client_qos);

    _publisher = _particpant->create_publisher(PUBLISHER_QOS_DEFAULT);

}

CommunicationManager::~CommunicationManager()
{
    for(const auto [name, topic] : _topics)
        _particpant->delete_topic(topic);

    _publisher->delete_contained_entities();
    _particpant->delete_publisher(_publisher);

    if(_particpant->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
        return;

    DomainParticipantFactory::get_instance()->delete_participant(_particpant);
}

int CommunicationManager::addDataWriter(std::string topicName)
{
    auto* writer = _publisher->create_datawriter(_topics.at(topicName), DATAWRITER_QOS_DEFAULT);
    _writers.push_back(writer);
    return _writers.size() - 1;
}

void CommunicationManager::writeData(int dataWriterID, void* data)
{
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto mill = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

    ((Header*)data)->timeSent(mill.count());
    std::cout << epoch.count() << std::endl;

    _writers[dataWriterID]->write(data, eprosima::fastrtps::rtps::InstanceHandle_t());
}
