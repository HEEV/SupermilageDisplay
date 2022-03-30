
#include "DataValues.h"

DataValues::DataValues() : ComManager(ComCenter((Delegate*)this)) {
	ComManager.initalize();
}
DataValues::~DataValues() {

}

void DataValues::updateHandler(std::string topic, std::string msg) {
	if (msg.length() > 4) {
		Test = msg;
	}
}