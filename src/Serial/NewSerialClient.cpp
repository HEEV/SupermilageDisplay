#define NewSerial
#define HardcodedSeiral

#define FASTDDS_NEW
//#define FASTDDS_OLD

#ifdef NewSerial

#include "Serial/NewSerialClient.h"

constexpr int RF_MAX_PACKET_SIZE = 251;

template <typename T>
void NewSerialClient::addPacket(T singlePacket, PACKET_TYPE type, bool isAvailable)
{
    unsigned long totalLength = (sizeof(singlePacket) + sizeof(type));
    if (isAvailable)
    {
        _serialOutput.write( (char*)&type, sizeof(type));
        _serialOutput.write( ((char*)&(*singlePacket)), sizeof(singlePacket));
    }
    else
    {
        for (int i = 0; i < totalLength; i++){_serialOutput << (char)0x00;}
    }
    totalBytes += (sizeof(singlePacket) + sizeof(type));
}

NewSerialClient::NewSerialClient(CommunicationManager &manager) : _comManager(manager)
{
    _activeSerial = Initalize();
}

NewSerialClient::~NewSerialClient()
{
    _serialOutput.close();
    _serialInput.close();
    _rfThread.join();
}

bool NewSerialClient::Initalize(std::string port, int BaudRate)
{
    #ifdef HardcodedSeiral
    _serialInput = std::ifstream("/dev/ttyACM0");//Opens the tty connection as an ifstream
    _serialOutput = std::ofstream("/dev/ttyACM0");//Opens the tty connection as an ofstream, not used in this example
    std::cout << "Serial was Initalized" << std::endl;
    return true;
    #endif
    #ifdef DynamicSerial         
    system("stty -F /dev/" + port + " cs8 " + std::to_string(BaudRate) + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    _serialInput = std::ifstream(("/dev/" + port));//Opens the tty connection as an ifstream
    _serialOutput = std::ofstream(("/dev/" + port));//Opens the tty connection as an ofstream, not used in this example
    #endif
}

void NewSerialClient::serialWrite()
{
    if (_activeSerial) 
    {      
        #ifdef FASTDDS_NEW
        _comManager.addDataReader<BatteryVoltage>("bat", [this](BatteryVoltage* battV)
        {
            _BatteryPackets.push(battV);
        });
        _comManager.addDataReader<CarTilt>("tilt", [this](CarTilt* tlt)
        {
            _CarTiltPackets.push(tlt);
        });
        _comManager.addDataReader<EngineTemp>("enTemp", [this](EngineTemp* temp)
        {
            _EngineTempPackets.push(temp);
        });
        _comManager.addDataReader<WheelData>("vel", [this](WheelData* wheelD)
        {
            _WheelDataPackets.push(wheelD);
        });
        _comManager.addDataReader<WindSpeed>("wind", [this](WindSpeed* windS)
        {
            _WindSpeedPackets.push(windS);
        });
        _comManager.addDataReader<GPSPosition>("gps", [this](GPSPosition* GPSPos)
        {
            _GPSPositionPackets.push(GPSPos);
        });

        _rfThread = std::thread(&NewSerialClient::SmooshNSend, this);

        #endif 
        #ifdef FASTDDS_OLD
        _comManager.addDataReader<BatteryVoltage>("bat", [this](BatteryVoltage* battV)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = VOLTAGE;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            std::cout << sizeof(tempPacketType) << std::endl;
            _serialOutput.write( ((char*)&(*battV)), sizeof(battV));
            totalBytes += (sizeof(battV) + sizeof(VOLTAGE));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        _comManager.addDataReader<CarTilt>("tilt", [this](CarTilt* tlt)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = TILT;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            _serialOutput.write( ((char*)&(*tlt)), sizeof(tlt));
            totalBytes = (sizeof(tlt) + sizeof(TILT));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        _comManager.addDataReader<EngineTemp>("enTemp", [this](EngineTemp* temp)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = TEMP;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            _serialOutput.write( ((char*)&(*temp)), sizeof(temp));
            totalBytes = (sizeof(temp) + sizeof(TEMP));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        _comManager.addDataReader<WheelData>("vel", [this](WheelData* wheelD)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = WHEEL;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            _serialOutput.write( ((char*)&(*wheelD)), sizeof(wheelD));
            totalBytes = (sizeof(wheelD) + sizeof(WHEEL));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        _comManager.addDataReader<WindSpeed>("wind", [this](WindSpeed* windS)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = WIND;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            _serialOutput.write( ((char*)&(*windS)), sizeof(windS));
            totalBytes = (sizeof(windS) + sizeof(WIND));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        _comManager.addDataReader<GPSPosition>("gps", [this](GPSPosition* GPSPos)
        {
            int totalBytes = 0;
            PACKET_TYPE tempPacketType = GPS;
            _serialOutput.write( (char*)&tempPacketType, sizeof(tempPacketType));
            _serialOutput.write( ((char*)&(*GPSPos)), sizeof(GPSPos));
            totalBytes += (sizeof(GPSPos) + sizeof(GPS));
            for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
        });
        #endif
    }
    else
    {
        std::cerr << "Failed to Initialize Serial" << '\n';
    }    
}

void NewSerialClient::SmooshNSend()
{
    PACKET_TYPE tempPacketType = NONE;
    totalBytes = 0;

    addPacket<BatteryVoltage*>(_BatteryPackets.front(), VOLTAGE, !_BatteryPackets.empty());
    _BatteryPackets.pop();

    addPacket<CarTilt*>(_CarTiltPackets.front(), TILT, !_CarTiltPackets.empty());
    _CarTiltPackets.pop();

    addPacket<EngineTemp*>(_EngineTempPackets.front(), TEMP, !_EngineTempPackets.empty());
    _EngineTempPackets.pop();

    addPacket<WheelData*>(_WheelDataPackets.front(), WHEEL, !_WheelDataPackets.empty());
    _WheelDataPackets.pop();

    addPacket<WindSpeed*>(_WindSpeedPackets.front(), WIND, !_WindSpeedPackets.empty());
    _WindSpeedPackets.pop();

    addPacket<GPSPosition*>(_GPSPositionPackets.front(), GPS, !_GPSPositionPackets.empty());
    _GPSPositionPackets.pop();

    for (int i = 0; i < RF_MAX_PACKET_SIZE - totalBytes; i++){_serialOutput << (char)0x00;}
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
