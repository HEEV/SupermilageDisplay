#pragma once
#include "mqttClient.h"
#include <string>
#include "Delegate.h"

#define Address "10.12.12.224:1883" //tcp://10.12.12.224:1883 old

class ComCenter : Delegate {
public:
	// CONSTRUCTORS
	ComCenter(Delegate* inst) : p_cellClient((Delegate*)this, Address) {
		p_Instance = inst;
	}

	bool initalize() {
		//setup the connetion with the mqtt module
		p_cellClient.initalize();

		while (p_cellClient.getState() != "0:Connected");
		//Test mqtt network connection
		p_cellClient.publish("Test2", "I am Alive");
		//Test Aysc Callback pointer if availible
		updateHandler("Hello Application Starting up", "");
		return true;
	}

	/*Gets the State of the mqtt clients
	@return a string that is the client connection status*/
	std::string getState() { 
		return p_cellClient.getState(); 
	}
	void updateHandler(std::string topic, std::string msg) {
		// This function handle async update from lower classes.the notifications are consolided and
		//filtered here so that the user only has to look at one function.

		p_Instance->updateHandler(topic, msg);
	}
	
private:
	std::string _Error;
	Delegate* p_Instance;
	mqttClient p_cellClient;
};

