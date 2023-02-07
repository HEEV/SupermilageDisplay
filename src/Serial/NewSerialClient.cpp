#ifdef NewSerial
    #include "Serial/NewSerialClient.h"

    NewSerialClient(uint32_t id, int64_t SentTime, float Velocity) :
    _id(id), _timeRec(0.0), _timeSent(SentTime), _velocity(Velocity)
    {

    }
	~NewSerialClient()
    {
        serialInput.close();
        serialOutput.close();
    }

    Initalize(std::string port, int BaudRate)
    {
    //    system("stty -F /dev/" + port +" cs8 " + (std::string)BaudRate + " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
    //    ifstream serialInput("/dev/" + port);//Opens the tty connection as an ifstream
    //    ofstream serialOutput("/dev/" + port);//Opens the tty connection as an ofstream, not used in this example
        system("stty -F /dev/ttyACM0 cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echokenoflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
        ifstream serialInput("/dev/ttyACM0");//Opens the tty connection as an ifstream
        ofstream serialOutput("/dev/ttyACM0");//Opens the tty connection as an ofstream, not used in this example
    }

	// Setters
	void setId(uint32_t id)
    {
        _id = id;
    }

	void setRecieve(int64_t ReceiveTime)
    {
        _timeRec = ReceiveTime;
    }

	void setSent(int64_t SentTime)
    {
        _timeSent = SentTime;
    }

	void setVelocity(float Velocity)
    {
        _velocity = Velocity;
    }

	// Serial IO
	void sendSerial()
    {
        serialOutput << _id << _timeRec << _timeSent << _veolcity << endl;
        serialOutput.clear();
    }

	void serialReceive()
    {
        serialInput >> _id >> _timeRec >> _timeSent >> _veolcity >> endl;
        serialInput.clear();
    }

#endif
