#ifndef LIDAR_DATA_STRUCTURES_H
#define LIDAR_DATA_STRUCTURES_H

/*
enum LIDAR_Directions
{
	front,		//316-45
	NorthEast,
	rightward,
	SouthEast,
	backward,
	SouthWest,
	leftward,
};
*/

enum LIDAR_Zones
{
	eight_zone,		//315-360 front
	first_zone,		//0-45 front
	second_zone, 	//45-90 rightward
	third_zone,		//90-135 rightward
	fourth_zone,	//135-180 backward
	fifth_zone,		//180-225 backward
	sixth_zone,		//225-270 leftward
	seventh_zone	//270-315 leftward
};

struct LIDAR_data_point
{
	float thetha;				//0-360
	float distance;				//cm or what?
	int measurement_quality; 	//0-100, 0 is unreliable and 100 is perfect
};

struct LIDAR_batch_scan_data 
{
	long long int timestamp_msec;
	int scanned_data_count;
	int lidar_ID;
	LIDAR_data_point LIDAR_data_point_array[720]; //dirt workaround is having set size of the array. I have to initialize that in my program. Change this.
};

LIDAR_Zones lidar_data_structures_getPointZone(LIDAR_data_point point);
void lidar_data_structures_printLidarPoint(LIDAR_data_point point);

#endif