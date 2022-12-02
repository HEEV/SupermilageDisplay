#include "Communication/CommunicationManager.h"

using namespace eprosima::fastdds::dds;

CommunicationManager::CommunicationManager()
{
    _particpant = 
        DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);

    _publisher = _particpant->create_publisher(PUBLISHER_QOS_DEFAULT);
}

CommunicationManager::~CommunicationManager()
{
    _publisher->delete_contained_entities();
    _particpant->delete_publisher(_publisher);

    if(_particpant->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
        return;

    DomainParticipantFactory::get_instance()->delete_participant(_particpant);
}
