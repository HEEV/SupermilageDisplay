#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <queue>
#include <thread>
#include <CommunicationManager.h>
#include "Packets.h" // If we can remove this that would be great, put this back in the cpp file

//constexpr char const* PORT = "ttyUSB0"; //"/dev/ttyUSB0"
constexpr char const* PORT = "ttyACM0"; //"/dev/ttyACM0"
constexpr int BAUDRATE = 115200; //9600

enum PACKET_TYPE {NONE, VOLTAGE, TILT, TEMP, WHEEL, WIND, GPS};


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
	template <typename T>
	void addPacket(T singlePacket, PACKET_TYPE type, bool isAvailable);

	CommunicationManager &_comManager;

	std::ifstream _serialInput;
	std::ofstream _serialOutput;

	int totalBytes;

	std::queue<BatteryVoltage*> _BatteryPackets;
	std::queue<CarTilt*> _CarTiltPackets;
	std::queue<EngineTemp*> _EngineTempPackets;
	std::queue<WheelData*> _WheelDataPackets;
	std::queue<WindSpeed*> _WindSpeedPackets;
	std::queue<GPSPosition*> _GPSPositionPackets;

	std::thread _rfThread;
	bool _activeSerial;
};
