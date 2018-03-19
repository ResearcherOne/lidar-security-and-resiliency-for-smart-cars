#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

class LoggerModule
{
	private:
		std::string filePath;
	public:
		LoggerModule(std::string);
		void log(std::string command);
};

#endif