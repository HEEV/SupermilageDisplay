#pragma once
#include <string>

struct SensorData{
	SensorData() {
		id = 'A';
		data = 12.345;
	}
	SensorData(char ID, float DATA) {
		id = ID;
		data = DATA;
	}
	char id;
	float data;
};

class Delegate {
public:
	virtual ~Delegate() {};
	virtual void updateHandler(std::string topic, SensorData msg) = 0;
};