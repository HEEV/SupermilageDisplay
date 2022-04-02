#pragma once

#include "../MQTTComunication/ComCenter.h"
#include "../MQTTComunication/Delegate.h"
#include <string>

class DataValues : Delegate {
public:
	DataValues();
	~DataValues();

	std::string Test = "MQTT Test";

	//Handles the incomming Data from the Communcation Manager
	void updateHandler(std::string topic, std::string msg) override;

private:
	//Communcation Manager Class
	ComCenter ComManager;

};