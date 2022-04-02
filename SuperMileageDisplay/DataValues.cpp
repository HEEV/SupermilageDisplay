
#include "DataValues.h"
#include <string>

DataValues::DataValues() : ComManager(ComCenter((Delegate*)this)) {
	ComManager.initalize();
}
DataValues::~DataValues() {

}

void DataValues::updateHandler(std::string topic, SensorData msg) {
	Test = topic;
	if (msg.id != 'E') {
		msg.data = msg.data / 10;
		if (msg.data > 30.0) {
			Value = 30.0;
		}
		else {
			Value = (msg.data);
		}
	}

#if defined (__linux__)
	if (topic == "Sensor") {
		ComManager.Publish(std::to_string(msg.id), msg);
	}
#endif
}