#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "../../rplidar/sdk/app/modules/util_module.hpp"

class LoggerModule
{
	private:
		std::string filePath;
		UtilModule util;
	public:
		LoggerModule(std::string);
		void log(std::string command);
};

#endif