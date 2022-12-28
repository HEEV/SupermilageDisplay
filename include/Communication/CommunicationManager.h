#pragma once
#include <map>
#include <string>
#include <concepts>
#include "FastDDS.h"

#define GET_TYPE_AS_STR(type) #type

class CommunicationManager
{
public:
    CommunicationManager();
    ~CommunicationManager();

    
    /// @brief  Register a new topic that can be written to or read from
    /// @param topicName Name of the topic to register
    /// @param dataTypeName Name of the data type the topic channel uses
    template <typename T>
    requires std::derived_from<T, eprosima::fastdds::dds::TopicDataType>
    void registerTopic(std::string topicName, const T& type);

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
void CommunicationManager::registerTopic(std::string topicName, const T& t)
{
    eprosima::fastdds::dds::TypeSupport type(new T());
    type.register_type(_particpant, type.get_type_name());
    auto* topic = _particpant->create_topic(topicName, type.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
    _topics.insert(std::make_pair(topicName, topic));
}
