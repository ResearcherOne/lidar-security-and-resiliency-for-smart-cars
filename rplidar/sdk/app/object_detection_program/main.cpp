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

void applyObjectDetectionRules(LIDAR_data_point resultPoint) {
	//printf("Point thetha %f, Point Distance cm %f, Quality of Measurement %d \n", resultPoint.thetha, resultPoint.distance, resultPoint.measurement_quality);
	float max_detection_range_in_cm = 40;

	float point_thetha = resultPoint.thetha;
	float point_distance = resultPoint.distance;
	if(point_distance < max_detection_range_in_cm) {
		//printf("An object is detected! %f at degrees, %f cm \n", point_thetha, point_distance);
		LIDAR_Zones detected_zone = lidar_data_structures_getPointZone(resultPoint);
		if(detected_zone == first_zone || detected_zone == eight_zone) {
			printf("The object is at the front zone! It is %f cm far at %f degrees. \n", point_distance, point_thetha);
		} else if (detected_zone == second_zone || detected_zone == third_zone) {
			printf("The object is at the right zone! It is %f cm far at %f degrees. \n", point_distance, point_thetha);
		} else if (detected_zone == fourth_zone || detected_zone == fifth_zone) {
			printf("The object is at the back zone! It is %f cm far at %f degrees. \n", point_distance, point_thetha);
		} else {
			printf("The object is at the left zone! It is %f cm far at %f degrees. \n", point_distance, point_thetha);
		}
	} else {
		printf("No object is in detection range! \n");
	}
	
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

	long long int timestamp = 500;
	while(!ctrl_c_pressed) {
		LIDAR_batch_scan_data lidar_batch_scan_data;
		lidar.grabBatchScanData(&lidar_batch_scan_data, lidar_ID, timestamp);

		LIDAR_data_point resultPoint;
		int quality_threshold = 5;

		ObjectDetectionModule *object_detection_module = new ObjectDetectionModule();
		object_detection_module->getClosestPoint(lidar_batch_scan_data.LIDAR_data_point_array, lidar_batch_scan_data.scanned_data_count, &resultPoint, quality_threshold);
	
		applyObjectDetectionRules(resultPoint);
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