#ifndef LIDAR_DATA_STRUCTURES_H
#define LIDAR_DATA_STRUCTURES_H

enum LIDAR_Directions
{
	North, //North is assumed to be where LIDAR is heading by default.
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest
};

enum LIDAR_Zones
{
	first_zone,		//NNE -> North, NorthEast
	second_zone, 	//ENE -> East, NorthEast
	third_zone,		//ESE -> East, SouthEast
	fourth_zone,	//SSE -> South, SourthEast
	fifth_zone,		//SSW
	sixth_zone,		//WSW
	seventh_zone,	//WNW
	eight_zone		//NNW
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

#endif