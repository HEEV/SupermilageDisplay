#pragma once

 #include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

//constexpr char const* PORT = "ttyUSB0"; //"/dev/ttyUSB0"
constexpr char const* PORT = "ttyACM0"; //"/dev/ttyACM0"

constexpr int BAUDRATE = 115200; //9600

class NewSerialClient {
public:
	NewSerialClient(uint32_t id, int64_t Sent, float Velocity);
	~NewSerialClient();
	Initalize(std::string port = PORT, int BaudRate = BAUDRATE);

	// Setters
	void setId(uint32_t id);
	void setReceive(int64_t Recieve);
	void setSent(int64_t Sent);
	void setVelocity(float Velocity);

	// Serial IO
	void sendSerial();
	void serialReceive();

private:
	uint32_t _id;
    int64_t _timeRec;
    int64_t _timeSent;
	float _velocity;

};