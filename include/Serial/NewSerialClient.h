#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

//constexpr char const* PORT = "ttyUSB0"; //"/dev/ttyUSB0"
constexpr char const* PORT = "ttyACM0"; //"/dev/ttyACM0"

constexpr int BAUDRATE = 115200; //9600

class NewSerialClient {
public:
	NewSerialClient();
	~NewSerialClient();
	bool Initalize(std::string port = PORT, int BaudRate = BAUDRATE);

	// Serial IO
	void serialWrite();
	void serialRead();

private:
	CommunicationManager::CommunicationManager& comManager;

	std::ifstream serialInput;
	std::ofstream serialOutput

	bool _activeSerial;
};