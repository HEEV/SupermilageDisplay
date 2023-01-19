#pragma once
#include <map>
#include <string>
#include <concepts>
#include "FastDDS.h"

#define REGISTER_TYPE_TO_MANAGER(type, topicName, manager)\
manager.registerTopic<type##PubSubType>(topicName)

class CommunicationManager
{
public:
    CommunicationManager();
    ~CommunicationManager();

    template <typename T>
    requires std::derived_from<T, eprosima::fastdds::dds::TopicDataType>
    void registerTopic(std::string topicName);

    /// @brief Create a dataWriter attached to a given topic
    /// @param topicName The topic name to attach the dataWriter to. Must
    /// have been created using registerTopic(...)
    /// @return The ID of the created data writer
    int addDataWriter(std::string topicName);

    /// @brief Write data using the given dataWriter
    /// @param dataWriterID The ID of the dataWriter to use
    /// @param data The data to write. The data must begin with the Header type
    void writeData(int dataWriterID, void* data);

private:
    eprosima::fastdds::dds::DomainParticipant* _particpant;
    eprosima::fastdds::dds::Publisher* _publisher;
    std::map<std::string, eprosima::fastdds::dds::Topic*> _topics;
    std::vector<eprosima::fastdds::dds::DataWriter*> _writers;

};

template <typename T>
requires std::derived_from<T, eprosima::fastdds::dds::TopicDataType>
inline void CommunicationManager::registerTopic(std::string topicName)
{
    eprosima::fastdds::dds::TypeSupport type(new T());
    _particpant->register_type(type);
    auto* topic =_particpant->create_topic(topicName, type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
    _topics.insert(std::make_pair(topicName, topic));
}
