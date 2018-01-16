#include <iostream>
#include "rplidar_module.hpp"
#include "LIDAR_data_structures.hpp"
#include "object_detection_module.hpp"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

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
	signal(SIGINT, ctrlc);

	sleepForMs(2000);
	lidar.startSystem();


	while(!ctrl_c_pressed) {
		LIDAR_batch_scan_data lidar_batch_scan_data;
		lidar.grabBatchScanData(&lidar_batch_scan_data);

		LIDAR_data_point resultPoint;
		int quality_threshold = 5;

		ObjectDetectionModule *object_detection_module = new ObjectDetectionModule();
		object_detection_module->getClosestPoint(lidar_batch_scan_data.LIDAR_data_point_array, lidar_batch_scan_data.scanned_data_count, &resultPoint, quality_threshold);
		printf("Point thetha %f, Point Distance %f, Quality of Measurement %d \n", resultPoint.thetha, resultPoint.distance, resultPoint.measurement_quality);
	}


	

	/*
	LIDAR_data_point resultPointArray[10];
	int number_of_requested_points = 10;
	object_detection_module->getClosestPointSet(lidar_batch_scan_data.LIDAR_data_point_array, lidar_batch_scan_data.scanned_data_count, resultPointArray, number_of_requested_points, quality_threshold);

	for(int i=0; i<number_of_requested_points; i++) {
		printf("Point thetha %f, Point Distance %f, Quality of Measurement %d \n", resultPointArray[i].thetha, resultPointArray[i].distance, resultPointArray[i].measurement_quality);
	}
	*/

	sleepForMs(2000);
	lidar.stopSystem();
	lidar.disposeRplidar();


	cout <<"Program is finished. \n";
	return 0;
}