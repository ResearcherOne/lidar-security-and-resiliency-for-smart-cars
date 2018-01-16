#ifndef OBJECT_DETECTION_MODULE_H
#define OBJECT_DETECTION_MODULE_H

#include <stdlib.h>
#include "LIDAR_data_structures.hpp"

class ObjectDetectionModule
{
	private:
		void cleanDataPointArrayFromUnqualifiedPoints(LIDAR_data_point LIDAR_data_point_array[], int size_of_array, LIDAR_data_point result_point_array[], int * result_data_point_count_ptr, int quality_threshold); // if (point_quality<=quality_threshold) dispose the point
		void sortDataPointArrayInAscendingOrderByDistance(LIDAR_data_point LIDAR_data_point_array[], LIDAR_data_point result_point_array[], int size_of_array);
	public:
		void getClosestPoint(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point * result_point, int quality_threshold);
		void getClosestPointSet(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point result_point_array[], int number_of_result_points, int quality_threshold);
		LIDAR_Zones getPointZone();
};

#endif