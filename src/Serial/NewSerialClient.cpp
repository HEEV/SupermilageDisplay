#define NewSerial
#define HardcodedSeiral

#ifdef NewSerial

#include "Serial/NewSerialClient.h"
#include "Packets.h"

enum PACKET_TYPE {NONE, VOLTAGE, TILT, TEMP, WHEEL, WIND, GPS};

NewSerialClient::NewSerialClient()
{
    _activeSerial = Initalize();
}

NewSerialClient::~NewSerialClient()
{
    _serialOutput.close();
    _serialInput.close();
}

bool NewSerialClient::Initalize(std::string port, int BaudRate)
{
    #ifdef HardcodedSeiral
    system("stty -F /dev/ttyACM0 cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    _serialInput = std::ifstream("/dev/ttyACM0");//Opens the tty connection as an ifstream
    _serialOutput = std::ofstream("/dev/ttyACM0");//Opens the tty connection as an ofstream, not used in this example
    #endif
    #ifdef DynamicSerial
    system("stty -F /dev/" + port + " cs8 " + std::to_string(BaudRate) + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    _serialInput = std::ifstream(("/dev/" + port));//Opens the tty connection as an ifstream
    _serialOutput = std::ofstream(("/dev/" + port));//Opens the tty connection as an ofstream, not used in this example
    #endif

    return true;
}

void NewSerialClient::serialWrite(CommunicationManager &comManager)
{
    if (_activeSerial) 
    {        
        comManager.addDataReader<BatteryVoltage>("bat", [this](BatteryVoltage* battV)
        {
            int totalBytes = 0;
            _serialOutput << VOLTAGE;
            _serialOutput.write( ((char*)&(*battV)), sizeof(battV));
            totalBytes += (sizeof(battV) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
        comManager.addDataReader<CarTilt>("tilt", [this](CarTilt* tlt)
        {
            int totalBytes = 0;
            _serialOutput << TILT;
            _serialOutput.write( ((char*)&(*tlt)), sizeof(tlt));
            totalBytes += (sizeof(tlt) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
        comManager.addDataReader<EngineTemp>("enTemp", [this](EngineTemp* temp)
        {
            int totalBytes = 0;
            _serialOutput << TEMP;
            _serialOutput.write( ((char*)&(*temp)), sizeof(temp));
            totalBytes += (sizeof(temp) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
        comManager.addDataReader<WheelData>("vel", [this](WheelData* wheelD)
        {
            int totalBytes = 0;
            _serialOutput << WHEEL;
            _serialOutput.write( ((char*)&(*wheelD)), sizeof(wheelD));
            totalBytes += (sizeof(wheelD) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
        comManager.addDataReader<WindSpeed>("wind", [this](WindSpeed* windS)
        {
            int totalBytes = 0;
            _serialOutput << WIND;
            _serialOutput.write( ((char*)&(*windS)), sizeof(windS));
            totalBytes += (sizeof(windS) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
        comManager.addDataReader<GPSPosition>("gps", [this](GPSPosition* GPSPos)
        {
            int totalBytes = 0;
            _serialOutput << GPS;
            _serialOutput.write( ((char*)&(*GPSPos)), sizeof(GPSPos));
            totalBytes += (sizeof(GPSPos) + 4);
            for (int i = totalBytes; i < 255; i++){_serialOutput << 0x00;}
        });
    }
    else
    {
        std::cerr << "Failed to Initialize Serial" << '\n';
    }    
}

void NewSerialClient::serialRead()
{
    if (_activeSerial)
    {
        PACKET_TYPE readType;
        _serialInput.read( (char*)&readType, sizeof(readType));

        // We will always send a PACKET_TYPE = NONE [0] at the end of every packet
        while (readType != NONE)
        {
            if (readType == VOLTAGE) 
            {
                BatteryVoltage battV;
                _serialInput.read( (char*)&battV, sizeof(battV));
                std::cout << "Battery Packet ID: " << battV.head().id() << std::endl;
            }
            else if (readType == TILT)
            {
                CarTilt carT;
                _serialInput.read( (char*)&carT, sizeof(carT));
                std::cout << "Car Tilt Packet ID: " << carT.head().id() << std::endl;
            }
            else if (readType == TEMP)
            {
                EngineTemp engineT;
                _serialInput.read( (char*)&engineT, sizeof(engineT));
                std::cout << "Engine Temperature Packet ID: " << engineT.head().id() << std::endl;
            }
            else if (readType == WHEEL)
            {
                WheelData wheelD;
                _serialInput.read( (char*)&wheelD, sizeof(wheelD));
                std::cout << "Velocity Packet ID: " << wheelD.head().id() << std::endl;
            }
            else if (readType == WIND)
            {
                WindSpeed windS;
                _serialInput.read( (char*)&windS, sizeof(windS));
                std::cout << "Wind Speed Packet ID: " << windS.head().id() << std::endl;
            }
            else if (readType == GPS)
            {
                GPSPosition GPS;
                _serialInput.read( (char*)&GPS, sizeof(GPS));
                std::cout << "GPS Packet ID: " << GPS.head().id() << std::endl;
            }
            else
            {
                readType = NONE;
            }
            _serialInput.read( (char*)&readType, sizeof(readType));
        }
    }
    else
    {
        std::cerr << "Failed to Initialize Serial" << '\n';
    }    
}
#endif
