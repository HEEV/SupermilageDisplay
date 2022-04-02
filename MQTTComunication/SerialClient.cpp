
#include "SerialClient.h"
#include "serialib.h"
#include "rs232.h"

SerialClient::SerialClient(Delegate* handle) {
	p_updateHandler = handle;
}

bool SerialClient::Initalize(std::string port, int BaudRate) {
	_AsyncSerial = std::thread(&SerialClient::polSerialPort, this, port, BaudRate);
	return true;
}

/*void SerialClient::polSerialPort(std::string port, int BaudRate) {
	serialib SerialPort = serialib();
	do {
		SerialPort.openDevice(port.c_str(), BaudRate);
	}
	while (!SerialPort.isDeviceOpen());

	SerialPort.DTR(true);
	SerialPort.RTS(false);

	int Length = 0;
	char Buffer[50];
	std::string Temp_Input = "";
	memset(Buffer, 0, 50);
	SerialPort.flushReceiver();
	while (SerialPort.isDeviceOpen() == true) {

		SerialPort.setDTR();
		Length = SerialPort.available();
		if (Length < 4) {
			continue;
		}
		//SerialPort.readBytes(Buffer, 5, 1000);
		SerialPort.readBytes(Buffer, Length);
		SerialPort.clearDTR();
		SerialPort.flushReceiver();
		Temp_Input = Buffer;

		p_updateHandler->updateHandler(Temp_Input, ConvertPayload(Buffer));
		memset(Buffer, 0, 50);
		
	}

	SerialPort.closeDevice();
}*/

void SerialClient::polSerialPort(std::string port, int bdrate) {
	int i = 0;
	int n = 0;
	int cport_nr = RS232_GetPortnr(port.c_str());        /* /dev/ttyS0 (COM1 on windows) */

	char mode[] = { '8','N','1', '\0'};
	std::string Temp_Input = "";
	unsigned char buf[20];


	while(RS232_OpenComport(cport_nr, bdrate, mode, 1)) {
		//Failed to Open
#ifdef _WIN32
		Sleep(1000);
#else
		usleep(1000000);  /* sleep for 1 Second */
#endif
	}

	bool fullmsg = false;
	RS232_flushRX(cport_nr);
	while (1) {
		n = RS232_PollComport(cport_nr, buf, 19);

		if (n > 0) {
			buf[n] = '\0';   /* always put a "null" at the end of a string! */
		}
		else {
			continue;
		}
		for (int i = 0; i < n; i++) {
			if (buf[i] == '\r') {
				buf[i] = '\0';
				fullmsg = true;
				break;
			}
			else if ((char)(buf[i]) < 32) {
				buf[i] = '0';
			}
		}
		Temp_Input += (char*)buf;
		memset(buf, 0, 20);
		if (!fullmsg) {
			continue;
		}
		Temp_Input.erase(0, Temp_Input.find_first_not_of('0'));
		

		if (Temp_Input != "") {
			p_updateHandler->updateHandler(Temp_Input, ConvertPayload(Temp_Input));
		}
		Temp_Input.clear();
		RS232_flushRX(cport_nr);
		fullmsg = false;
	}
}

SensorData SerialClient::ConvertPayload(std::string Temp_Input) {
	SensorData Temp_Struct = SensorData();
	if (Temp_Input.length() > 1) {
		Temp_Struct.id = Temp_Input[0];
		try {
			Temp_Struct.data = std::stof(Temp_Input.substr(1, Temp_Input.length() - 1));
		}
		catch (std::invalid_argument e) {
			Temp_Struct.id = 'E';
		}	
		catch (std::out_of_range ee) {
			Temp_Struct.id = 'E';
		}
	}
	return Temp_Struct;
}