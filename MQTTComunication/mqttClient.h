#pragma once
//Download Instructions for mosquitto libarary at https://mosquitto.org/download/
//Library Documentation at https://mosquitto.org/man/libmosquitto-3.html

//For compliation "-lmosquitto" needs to be added on the commandline arguments to include the libaray once installed on the remote linux machine

#include <string>
#include "Delegate.h"
#include "mosquitto.h"

//Static Connection Paramaters
#define keepalive 60
#define clean_session true

class mqttClient {
public:
	mqttClient();
	mqttClient(Delegate* inst, std::string BrokerAddress);
	~mqttClient();

	bool initalize();
	bool Connect();
	void publish(std::string topic, std::string msg);
	std::string getState();

	void OnLogEvent(struct mosquitto* mosq, void* userdata, int level, const char* str);

	void OnConnected(struct mosquitto* mosq, void* userdata, int result);

	void OnSubcribed(struct mosquitto* mosq, void* userdata, int mid, int qos_count, const int* granted_qos);

	Delegate* p_Instance;
	struct mosquitto* mosq;

private:
	std::string _State;
	std::string _BrokerAddress;
	int _Port;
};
