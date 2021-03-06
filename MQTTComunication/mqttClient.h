#pragma once
//Download Instructions for mosquitto libarary at https://mosquitto.org/download/
//Library Documentation at https://mosquitto.org/man/libmosquitto-3.html

//For compliation "-lmosquitto" needs to be added on the commandline arguments to include the libaray once installed on the remote linux machine

#include <string>
#include "Delegate.h"
#include "mosquitto.h"

//Static Connection Paramaters
constexpr int keepalive = 60;
constexpr bool clean_session = true;
constexpr char const* DEFAULT_ADRESS = "127.0.0.1";

//State Codes:
constexpr char const* STATE0 = "0:Connected";
constexpr char const* STATE1 = "1:Uninitalized";
constexpr char const* STATE2 = "2:Unable to connect";
constexpr char const* STATE3 = "3:Connection Failed";


class mqttClient {
public:
	mqttClient();
	mqttClient(Delegate* inst, std::string BrokerAddress, std::string Username = "", std::string Password = "");
	~mqttClient();

	bool initalize();
	bool Connect();
	void publish(std::string topic, SensorData msg);
	std::string getState();

	void OnLogEvent(struct mosquitto* mosq, void* userdata, int level, const char* str);

	void OnConnected(struct mosquitto* mosq, void* userdata, int result);

	void OnSubcribed(struct mosquitto* mosq, void* userdata, int mid, int qos_count, const int* granted_qos);

	Delegate* p_Instance;
	struct mosquitto* mosq;

private:
	std::string _State;
	std::string _BrokerAddress;
	std::string _Username;
	std::string _Password;
	bool _Secure = false;
	int _Port;
};
