#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <queue>
#include <CommunicationManager.h>

//constexpr char const* PORT = "ttyUSB0"; //"/dev/ttyUSB0"
constexpr char const* PORT = "ttyACM0"; //"/dev/ttyACM0"

constexpr int BAUDRATE = 115200; //9600

class NewSerialClient {
public:
	NewSerialClient(CommunicationManager &manager);
	~NewSerialClient();
	bool Initalize(std::string port = PORT, int BaudRate = BAUDRATE);

	// Serial IO
	void serialWrite();
	void serialRead();
	void SmooshNSend();

private:
	CommunicationManager &_comManager;

	std::ifstream _serialInput;
	std::ofstream _serialOutput;

	int totalBytes;

	std::queue<BatteryVoltage*>   _BatteryPackets;
	std::queue<CarTilt*>          _CarTiltPackets;
	std::queue<EngineTemp*>       _EngineTempPackets;
	std::queue<WheelData*>        _WheelDataPackets;
	std::queue<WindSpeed*>        _WindSpeedPackets;
	std::queue<GPSPosition*>      _GPSPositionPackets;

	bool _activeSerial;
};
