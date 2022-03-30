#pragma once
#include <string>

class Delegate {
public:
	virtual ~Delegate() {};
	virtual void updateHandler(std::string topic, std::string msg) = 0;
};