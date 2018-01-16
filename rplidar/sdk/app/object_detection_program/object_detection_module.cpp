#include "object_detection_module.hpp"
#include "LIDAR_data_structures.hpp"

#include <stdio.h>
#include <stdlib.h>


void cleanDataPointArrayFromUnqualifiedPoints(LIDAR_data_point LIDAR_data_point_array[], int size_of_array, LIDAR_data_point result_point_array[], int * result_data_point_count, int quality_threshold)
{

} // if (point_quality<=quality_threshold) dispose the point

void ObjectDetectionModule::sortDataPointArrayInAscendingOrderByDistance(LIDAR_data_point LIDAR_data_point_array[], LIDAR_data_point result_point_array[], int size_of_array)
{

}

void ObjectDetectionModule::getClosestPoint(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point * result_point, int quality_threshold)
{
	//cleanData
	//sortData
	//return first point
}
void ObjectDetectionModule::getClosestPointSet(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point result_point_array[], int number_of_result_points, int quality_threshold)
{
	//cleanData
	//sortData
	//return first X number of points
}

LIDAR_Zones ObjectDetectionModule::getPointZone()
{

}