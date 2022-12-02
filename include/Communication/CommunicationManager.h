#pragma once
#include <vector>
#include "FastDDS.h"


class CommunicationManager
{
public:
    CommunicationManager();
    ~CommunicationManager();

    void registerTopic();

private:
    eprosima::fastdds::dds::DomainParticipant* _particpant;
    eprosima::fastdds::dds::Publisher* _publisher;


};
