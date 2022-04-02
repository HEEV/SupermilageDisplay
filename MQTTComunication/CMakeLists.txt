cmake_minimum_required(VERSION 3.15)

target_compile_features(SuperMileageDisplay PRIVATE cxx_std_20)
target_sources(SuperMileageDisplay PRIVATE 
	CommsInterface.h
	Delegate.h
	ComCenter.h
	mqttClient.h
	mqttClient.cpp
	SerialClient.h
	SerialClient.cpp
	serialib.h
	serialib.cpp
	rs232.c
	rs232.h
)