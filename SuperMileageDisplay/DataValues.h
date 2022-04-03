#pragma once

#include "ComCenter.h"
#include "Delegate.h"
#include <string>

class DataValues : Delegate {
public:
	DataValues();
	~DataValues();

	std::string Test = "MQTT Test";
	float Value = 0.0;

	//Handles the incomming Data from the Communcation Manager
	void updateHandler(std::string topic, SensorData msg) override;

private:
	//Communcation Manager Class
	ComCenter ComManager;

};