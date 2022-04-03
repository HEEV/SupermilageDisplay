#pragma once

#include <string>
#include <thread>
#include <mutex>
#include "Delegate.h"

#if defined (_WIN32) || defined( _WIN64)
	#define PORT "COM4"
#endif
#if defined (__linux__)
	#define PORT  "/dev/ttyUSB0"
#endif
#define BAUDRATE 115200 //9600

class SerialClient {
public:
	SerialClient(Delegate* handle);
	~SerialClient();
	bool Initalize(std::string port = PORT, int BaudRate = BAUDRATE);

private:
	void polSerialPort(std::string port, int BaudRate);
	SensorData ConvertPayload(std::string Temo_Input);

	Delegate* p_updateHandler;
	bool _EndRead = false;
	std::thread _AsyncSerial;
};