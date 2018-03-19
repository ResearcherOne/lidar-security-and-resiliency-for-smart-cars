#include <iostream>
#include <stdlib.h>
#include "logger_module.hpp"
#include <string>
#include <sstream>
#include <stdio.h>
#include "../../rplidar/sdk/app/02_scan_and_save/util_module.hpp"

UtilModule util;

template <typename T>
std::string toString(T val)
{
   	std::stringstream stream;
    stream << val;
    return stream.str();
}

LoggerModule::LoggerModule(std::string filePath)
{
	this->filePath = filePath;
}

void LoggerModule::log(std::string message)
{
	long long int current_millis = util.getCurrentTimeMilliseconds();
	std::cout << current_millis << ": "<< message.c_str() << "\n";
	std::string command = "sudo echo \""+toString(current_millis)+": "+message+"\" >> "+this->filePath;
	system(command.c_str());
}