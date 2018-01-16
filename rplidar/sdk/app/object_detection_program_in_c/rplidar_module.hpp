#ifndef RPLIDAR_MODULE_H
#define RPLIDAR_MODULE_H

#include "rplidar.h"

struct RplidarModule
{
	int lidar_ID;
	bool is_scanning;
	rp::standalone::rplidar::RPlidarDriver * drv; //Shall I set this as private field in the class??
};

bool RplidarModule_initializeSystem(struct RplidarModule *rplidar , int lidar_ID);
void RplidarModule_startSystem(struct RplidarModule *rplidar);
void RplidarModule_grabBatchScanData(struct RplidarModule *rplidar); //Create LIDAR_DATA_STRUCTURES.h Creating my own header will let me use other LIDAR with minimal change to my code.
void RplidarModule_stopSystem(struct RplidarModule *rplidar);
void RplidarModule_disposeRplidar(struct RplidarModule *rplidar);

#endif