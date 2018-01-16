#include <iostream>
#include "rplidar_module.hpp"

#include <unistd.h>


using namespace std;

void sleepForMs(unsigned int milliseconds)
{
	usleep(milliseconds*1000);
}

int main()
{
	int lidar_ID = 8;
	cout <<"This program will create rplidar_module with ID "<<lidar_ID<<" \n";
	
	RplidarModule lidar(lidar_ID);

	lidar.initializeHardware();
	sleepForMs(5000);
	lidar.startSystem();

	lidar.grabBatchScanData();

	sleepForMs(5000);
	lidar.stopSystem();
	lidar.disposeRplidar();


	cout <<"Program is finished. \n";
	return 0;
}