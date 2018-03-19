#include <iostream>
#include <stdlib.h>
#include "logger_module.hpp"
#include <string>
#include <sstream>
#include <stdio.h>

LoggerModule::LoggerModule(std::string filePath)
{
	this->filePath = filePath;
}

void LoggerModule::log(std::string message)
{
	std::cout << message.c_str() << "\n";
	std::string command = "sudo echo \""+message+"\" >> ./logger_module.log";
	system(command.c_str());
}