
#include "SerialClient.h"
#include <rs232.h>
#include <stdio.h>

#include "Profiler.h"

SerialClient::SerialClient(Delegate* handle) {
	p_updateHandler = handle;
}

bool SerialClient::Initalize(std::string port, int BaudRate) {
	FUNCTION_PROFILE();
#if defined (__linux__)
	_AsyncSerial = std::thread(&SerialClient::polSerialPort, this, port, BaudRate);
#endif
	return true;
}

SerialClient::~SerialClient() {
	FUNCTION_PROFILE();
	_EndRead = true;
#if defined (__linux__)
	_AsyncSerial.join();
#endif
}

void SerialClient::polSerialPort(std::string port, int bdrate) {
	FUNCTION_PROFILE();
#if defined (__linux__)
	int n = 0;

	int cport_nr = RS232_GetPortnr(port.c_str());
	char mode[] = { '8','N','1', '\0'};

	char buf[50];


	while(RS232_OpenComport(cport_nr, bdrate, mode, 1)) {
		//Failed to Open
		if (_EndRead) {
			return;
		}
		if (n) {
			cport_nr--;
			n = 0;
		}
		else {
			cport_nr++;
			n = 1;
		}

#ifdef _WIN32
		Sleep(1000);
#else
		usleep(1000000);  /* sleep for 1 Second */
#endif
	}

	n = 0;
	char* chunkEnd = nullptr;
	char* sectEnd = nullptr;
	std::string Input;
	ptrdiff_t end = 0;
	int pos = 0;
	int carry = 0;
	ptrdiff_t index = 0;
	
	RS232_flushRX(cport_nr);
	while (!_EndRead) {
		n = RS232_PollComport(cport_nr, (unsigned char*)&buf[carry], 49 - carry);
	
		buf[n + carry] = '\0';   /* always put a "null" at the end of a string! */
		
		chunkEnd = strrchr(buf, '\n');

		if (chunkEnd == nullptr) {
			carry += n;
			continue;
		}

		chunkEnd++;
		index = (chunkEnd - buf);
		
		pos = 0;
		while ((sectEnd = (char*)memchr(&buf[pos], '\r', index)) != nullptr) {
			end = sectEnd - buf;
			Input = std::string(&buf[pos], end - pos);
			p_updateHandler->updateHandler("Sensor", ConvertPayload(Input));

			pos = end + 2;
		}

		carry = (n + carry) - index;
		strncpy(buf, chunkEnd, carry);
	}

	RS232_CloseComport(cport_nr);
#endif
}

SensorData SerialClient::ConvertPayload(std::string& Temp_Input) {
	FUNCTION_PROFILE();
	SensorData Temp_Struct = SensorData();
	if (Temp_Input.length() > 1) {
		Temp_Struct.id = Temp_Input[0];
		try {
			Temp_Struct.data = std::stof(Temp_Input.substr(1, Temp_Input.length() - 1));
		}
		catch (std::invalid_argument e) {
			Temp_Struct.id = 'A';
		}	
		catch (std::out_of_range ee) {
			Temp_Struct.id = 'A';
		}
	}
	return Temp_Struct;
}