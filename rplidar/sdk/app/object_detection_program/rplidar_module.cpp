#include <iostream>
#include "rplidar_module.h"

using namespace std;


bool RplidarModule::initializeSystem(int lidar_no)
{
	bool isInitialized = false;
	lidar_ID = lidar_no;
	return isInitialized;
}

void RplidarModule::startSystem()
{
	cout << "RplidarModule "<< lidar_ID <<": starting the system! \n";
}

void RplidarModule::stopSystem()
{
	cout << "RplidarModule "<< lidar_ID <<": stopping the system!! \n";
}

void RplidarModule::grabBatchScanData()
{
	cout << "RplidarModule "<< lidar_ID <<": grabbing batch scan data. \n";
}