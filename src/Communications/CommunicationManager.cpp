#include "Communication/CommunicationManager.h"
#include <istream>
#include <chrono>
#include "Communication/PacketTypes/header.h"

using namespace eprosima::fastdds::dds;

CommunicationManager::CommunicationManager()
{
    DomainParticipantQos qos;
    eprosima::fastrtps::rtps::RemoteServerAttributes server;
    eprosima::fastrtps::rtps::Locator_t loc;
    qos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT;
    std::istringstream("44.53.00.5f.45.50.52.4f.53.49.4d.41") >> qos.wire_protocol().prefix;
    eprosima::fastrtps::rtps::IPLocator::setIPv4(loc, "127.0.0.1");
    loc.port = 25565;
    server.metatrafficUnicastLocatorList.push_back(loc);
    server.ReadguidPrefix("11.11.11.11.11.11.11.11.11.11.11.11");
    qos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(server);
    qos.wire_protocol().builtin.discovery_config.discoveryServer_client_syncperiod = eprosima::fastrtps::Duration_t(0, 250000000);

    _particpant = 
        DomainParticipantFactory::get_instance()->create_participant(0, qos);

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
    auto nowMS = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = nowMS.time_since_epoch();

    ((Header*)data)->timeSent(std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count());

    _writers[dataWriterID]->write(data, eprosima::fastrtps::rtps::InstanceHandle_t());
}
