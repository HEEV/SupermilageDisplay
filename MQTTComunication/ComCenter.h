#pragma once
#include <string>
#include "mqttClient.h"
#include "Delegate.h"
#include "SerialClient.h"
#include "Profiler.h"

constexpr char const* Address = "10.12.12.224:1883"; //tcp://10.12.12.224:1883 old

class ComCenter : Delegate {
public:
	// CONSTRUCTORS
	ComCenter(Delegate* inst) : p_cellClient((Delegate*)this, Address), _SerialClient((Delegate*)this) {
		FUNCTION_PROFILE();
		p_Instance = inst;
	}

	bool initalize() {
		FUNCTION_PROFILE();
		//setup the connetion with the mqtt module
		p_cellClient.initalize();

		while (p_cellClient.getState() != "0:Connected");
		//Test mqtt network connection
		p_cellClient.publish("ComCenter", SensorData());

		_SerialClient.Initalize();

		return true;
	}

	/*Gets the State of the mqtt clients
	@return a string that is the client connection status*/
	std::string getState() { 
		FUNCTION_PROFILE();
		return p_cellClient.getState(); 
	}

	//Sends topic and message out all connected channels
	void Publish(std::string topic, SensorData message) {
		FUNCTION_PROFILE();
		p_cellClient.publish(topic, message);
	}

	void updateHandler(std::string topic, SensorData msg) {
		FUNCTION_PROFILE();
		// This function handle async update from lower classes.the notifications are consolided and
		//filtered here so that the user only has to look at one function.
		if (msg.id == 'E') {
			return;
		}

		p_Instance->updateHandler(topic, msg);

#if defined (__linux__)
		if (topic == "Sensor") {
			Publish(std::to_string(msg.id), msg);
		}
#endif
	}
	
private:
	std::string _Error;
	Delegate* p_Instance;
	mqttClient p_cellClient;
	SerialClient _SerialClient;
};

