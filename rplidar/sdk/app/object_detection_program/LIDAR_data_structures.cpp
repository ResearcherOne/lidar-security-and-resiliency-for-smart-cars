#include "LIDAR_data_structures.hpp"
#include <stdio.h>

LIDAR_Zones lidar_data_structures_getPointZone(LIDAR_data_point point)
{
	float thetha = point.thetha;
	if(thetha>=0 && thetha<45) {
		return first_zone;
	} else if(thetha>=45 && thetha<90) {
		return second_zone;
	} else if(thetha>=90 && thetha<135) {
		return third_zone;
	} else if(thetha>=135 && thetha<180) {
		return fourth_zone;
	} else if(thetha>=180 && thetha<225) {
		return fifth_zone;
	} else if(thetha>=225 && thetha<270) {
		return sixth_zone;
	} else if(thetha>=270 && thetha<315) {
		return seventh_zone;
	} else {
		return eight_zone;
	}
}

void lidar_data_structures_printLidarPoint(LIDAR_data_point point)
{
	printf("Point thetha %f, Point Distance %f, Quality of Measurement %d \n", point.thetha, point.distance, point.measurement_quality);
}