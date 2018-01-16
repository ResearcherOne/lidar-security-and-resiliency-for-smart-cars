#include <iostream>
#include "rplidar_module.hpp"
#include "LIDAR_data_structures.hpp"

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
	sleepForMs(2000);
	lidar.startSystem();

	LIDAR_batch_scan_data lidar_batch_scan_data;
	lidar.grabBatchScanData(&lidar_batch_scan_data);

	sleepForMs(2000);
	lidar.stopSystem();
	lidar.disposeRplidar();


	cout <<"Program is finished. \n";
	return 0;
}