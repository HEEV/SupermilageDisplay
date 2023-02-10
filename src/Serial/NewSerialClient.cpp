#define NewSerial
#ifdef NewSerial

#include "Serial/NewSerialClient.h"
#include "Packets.h"

enum PACKET_TYPE {NONE, VOLTAGE, TILT, TEMP, WHEEL, WIND, GPS};

NewSerialClient::NewSerialClient(CommunicationManager &m)
{
    comManager = m;
    _activeSerial = Initalize();
}

NewSerialClient::~NewSerialClient()
{
    serialOutput.close();
    serialInput.close();
}

bool NewSerialClient::Initalize(std::string port, int BaudRate)
{
    #ifdef HardcodedSeiral
    system("stty -F /dev/ttyACM0 cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    serialInput = std::ifstream("/dev/ttyACM0");//Opens the tty connection as an ifstream
    serialOutput = std::ofstream("/dev/ttyACM0");//Opens the tty connection as an ofstream, not used in this example
    #endif

    system("stty -F /dev/" + port + " cs8 " + to_string(Baudrate) + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    ifstream serialInput(("/dev/" + port));//Opens the tty connection as an ifstream
    ofstream serialOutput(("/dev/" + port));//Opens the tty connection as an ofstream, not used in this example

    return true;
}

void NewSerialClient::serialWrite()
{
    if (_activeSerial) 
    {
        PACKET_TYPE temp;
        
        comManager._manager.addDataReader<BatteryVoltage>("bat", [this](BatteryVoltage* battV)
        {
            int totalBytes = 0;
            serialOutput << (temp = VOLTAGE);
            serialOutput.write( ((char*)&(*battV)), sizeof(battV));
            totalBytes += (sizeof(battV) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
        });
        comManager._manager.addDataReader<CarTilt>("tilt", [this](CarTilt* carT)
        {
            int totalBytes = 0;
            serialOutput << (temp = TILT);
            serialOutput.write( ((char*)&(*carT)), sizeof(carT));
            totalBytes += (sizeof(carT) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
        });
        comManager._manager.addDataReader<engineTemp>("enTemp", [this](engineTemp* engineT)
        {
            int totalBytes = 0;
            serialOutput << (temp = TEMP);
            serialOutput.write( ((char*)&(*engineT)), sizeof(engineT));
            totalBytes += (sizeof(engineT) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
        });
        comManager._manager.addDataReader<wheelData>("vel", [this](wheelData* wheelD)
        {
            int totalBytes = 0;
            serialOutput << (temp = WHEEL);
            serialOutput.write( ((char*)&(*wheelD)), sizeof(wheelD));
            totalBytes += (sizeof(wheelD) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
        });
        comManager._manager.addDataReader<windSpeed>("wind", [this](windSpeed* windS)
        {
            int totalBytes = 0;
            serialOutput << (temp = WIND);
            serialOutput.write( ((char*)&(*windS)), sizeof(windS));
            totalBytes += (sizeof(windS) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
        });
        comManager._manager.addDataReader<GPSPosition>("gps", [this](GPSPosition* GPS)
        {
            int totalBytes = 0;
            serialOutput << (temp = GPS);
            serialOutput.write( ((char*)&(*GPS)), sizeof(GPS));
            totalBytes += (sizeof(GPS) + 4);
            for (int i = totalBytes; i < 255; i++){serialOutput << 0x00;}
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
        serialInput.read( (char*)&readType, sizeof(readType));

        // We will always send a PACKET_TYPE = NONE [0] at the end of every packet
        while (readType != 0)
        {
            switch (readType)
            {
            case 1:
                BatteryVoltage battV;
                serialInput.read( (char*)&battV, sizeof(battV));
                cout << "Battery Packet ID: " << battV->head()->id() << endl;
                break;
            case 2:
                CarTilt carT;
                serialInput.read( (char*)&carT, sizeof(carT));
                cout << "Car Tilt Packet ID: " << carT->head()->id() << endl;
                break;
            case 3:
                engineTemp engineT;
                serialInput.read( (char*)&engineT, sizeof(engineT));
                cout << "Engine Temperature Packet ID: " << engineT->head()->id() << endl;
                // WIP add new data "poster"
                break;
            case 4:
                wheelData wheelD;
                serialInput.read( (char*)&wheelD, sizeof(wheelD));
                cout << "Velocity Packet ID: " << wheelD->head()->id() << endl;
                // WIP add new data "poster"
                break;
            case 5:
                windSpeed windS;
                serialInput.read( (char*)&windS, sizeof(windS));
                cout << "Wind Speed Packet ID: " << windS->head()->id() << endl;
                // WIP add new data "poster"
                break;
            case 6:
                GPSPosition GPS;
                serialInput.read( (char*)&GPS, sizeof(GPS));
                cout << "GPS Packet ID: " << GPS->head()->id() << endl;
                // WIP add new data "poster"
                break;
            default:
                readType = NONE;
                break;
            }
            serialInput.read( (char*)&readType, sizeof(readType));
        }
    }
    else
    {
        std::cerr << "Failed to Initialize Serial" << '\n';
    }    
}
#endif
