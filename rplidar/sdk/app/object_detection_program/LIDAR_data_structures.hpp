#ifndef LIDAR_DATA_STRUCTURES_H
#define LIDAR_DATA_STRUCTURES_H

struct LIDAR_data_point {
	float thetha;				//0-360
	float distance;				//cm or what?
	int measurement_quality; 	//0-100, is unreliable 100 is perfect
};

struct LIDAR_batch_scan_data {
	long long int timestamp_msec;
	//LIDAR_ID
	//LIDAR data point array
};

#endif