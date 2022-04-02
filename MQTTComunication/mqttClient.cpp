
#include "mqttClient.h"
#include <cstring>

mqttClient* Global_Connection[5] = { 0 };

void Logevent(struct mosquitto* mosq, void* userdata, int level, const char* str);
void Onconnect(struct mosquitto* mosq, void* userdata, int level);
void Global_Message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* message);
void Subscribe(struct mosquitto* mosq, void* userdata, int mid, int qos_count, const int* granted_qos);
void Global_Reconnect(struct mosquitto* mosq, void* data, int num);

mqttClient::mqttClient() {
	p_Instance = nullptr;
	mosq = nullptr;
	_State = STATE1;
	_BrokerAddress = DEFAULT_ADRESS;
	_Port = 1883;
	_Secure = false;
}

mqttClient::mqttClient(Delegate* inst, std::string BrokerAddress, std::string Username, std::string Password) {
	_BrokerAddress = BrokerAddress.substr(0, BrokerAddress.find(':'));
	_Port = stoi(BrokerAddress.substr(BrokerAddress.find(':') + 1));
	mosq = nullptr;
	p_Instance = inst;
	for (int i = 0; i < 5; i++) {
		if (Global_Connection[i] == nullptr) {
			Global_Connection[i] = this;
			break;
		}
	}

	if (Username == "" && Password == "") {
		_Username = Username;
		_Password = Password;
		_Secure = true;
	}
	else {
		_Secure = false;
	}
}

mqttClient::~mqttClient() {
	mosquitto_loop_stop(mosq, true);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}

bool mqttClient::initalize() {
	if (_State == STATE1) {
		return false;
	}
	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, NULL);
	mosquitto_log_callback_set(mosq, Logevent);
	mosquitto_connect_callback_set(mosq, Onconnect);
	mosquitto_message_callback_set(mosq, Global_Message);
	mosquitto_subscribe_callback_set(mosq, Subscribe);
	mosquitto_disconnect_callback_set(mosq, Global_Reconnect);
	if (_Secure) {
		mosquitto_username_pw_set(mosq, _Username.c_str(), _Password.c_str());
	}

	return Connect();
}
bool mqttClient::Connect() {
	//Abort if class has not been initalized
	if (_State == STATE1) {
		return false;
	}
	//connect to the mqtt broker, responce will be given in the Connected methode
	if (mosquitto_connect_async(mosq, _BrokerAddress.c_str(), _Port, keepalive)) {
		_State = STATE2;
		return false;
	}

	//maintains the connection in seperate thread, this thread is closed in the Destructor
	mosquitto_loop_start(mosq);
	return true;
}
void mqttClient::publish(std::string topic, std::string msg) {
	//function definition: int mosquitto_publish(mosq, mid, topic, payloadlen, payload, qos, retain);
	mosquitto_publish(mosq, 0, topic.c_str(), msg.size(), msg.c_str(), 1, false);
}

std::string mqttClient::getState() { 
	return _State;
}

void mqttClient::OnLogEvent(struct mosquitto* mosq, void* userdata, int level, const char* str) {

}

void mqttClient::OnConnected(struct mosquitto* mosq, void* userdata, int result) {
	if (!result) {
		//Successful Connect
		_State = STATE0;
		//Subscribe in incomming topics
		mosquitto_subscribe(mosq, NULL, "#", 2);
	}
	else {
		_State = STATE3;
	}
}

void mqttClient::OnSubcribed(struct mosquitto* mosq, void* userdata, int mid, int qos_count, const int* granted_qos) {

}

//Golobal Functions
void Logevent(struct mosquitto* mosq, void* userdata, int level, const char* str) {
	for (int i = 0; i < 5; i++) {
		if (Global_Connection[i] != nullptr && Global_Connection[i]->mosq == mosq) {
			Global_Connection[i]->OnLogEvent(mosq, userdata, level, str);
			break;
		}
	}
}
void Onconnect(struct mosquitto* mosq, void* userdata, int level) {
	for (int i = 0; i < 5; i++) {
		if (Global_Connection[i] != nullptr && Global_Connection[i]->mosq == mosq) {
			Global_Connection[i]->OnConnected(mosq, userdata, level);
			break;
		}
	}
}
void Global_Message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* message) {
	for (int i = 0; i < 5; i++) {
		if (Global_Connection[i] != nullptr && Global_Connection[i]->mosq == mosq) {
			//Filltering for incomming message
			std::string topic = std::string(message->topic);
			std::string msg = "null";
			//payload is a void* so it must be copied into c-string buffer
			if (message->payloadlen > 0) {
				char* msgTemp = new char[message->payloadlen + 1];
				std::memcpy(msgTemp, message->payload, message->payloadlen);
				msgTemp[message->payloadlen] = '\0';
				msg = std::string(msgTemp);
			}

			Global_Connection[i]->p_Instance->updateHandler(topic, msg);
			break;
		}
	}
}
void Subscribe(struct mosquitto* mosq, void* userdata, int mid, int qos_count, const int* granted_qos) {
	for (int i = 0; i < 5; i++) {
		if (Global_Connection[i] != nullptr && Global_Connection[i]->mosq == mosq) {
			Global_Connection[i]->OnSubcribed(mosq, userdata, mid, qos_count, granted_qos);
			break;
		}
	}
}

void Global_Reconnect(struct mosquitto* mosq, void* data, int num) {
	mosquitto_reconnect_async(mosq);
}