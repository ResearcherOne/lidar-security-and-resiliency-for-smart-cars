#include "object_detection_module.hpp"
#include "LIDAR_data_structures.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

void ObjectDetectionModule::clearDataPointArrayFromUnqualifiedPoints(LIDAR_data_point LIDAR_data_point_array[], int size_of_array, LIDAR_data_point result_point_array[], int * result_data_point_count, int quality_threshold)
{
	int number_of_clean_data_points = 0;
	for (int i = 0; i < size_of_array; i++) {
		LIDAR_data_point current_point = LIDAR_data_point_array[i];
		if(current_point.measurement_quality > quality_threshold) {
			result_point_array[number_of_clean_data_points].thetha = current_point.thetha;
			result_point_array[number_of_clean_data_points].distance = current_point.distance;
			result_point_array[number_of_clean_data_points].measurement_quality = current_point.measurement_quality;
			number_of_clean_data_points++;
		} else {
			//dispose the point
		}
	}
	*result_data_point_count = number_of_clean_data_points;
}

bool compareDataPointsByDistance(LIDAR_data_point lhs, LIDAR_data_point rhs) { return lhs.distance < rhs.distance; }

void ObjectDetectionModule::sortDataPointArrayInAscendingOrderByDistance(LIDAR_data_point LIDAR_data_point_array[], int size_of_array)
{
	std::sort(LIDAR_data_point_array, LIDAR_data_point_array+size_of_array, compareDataPointsByDistance);
}

void ObjectDetectionModule::getClosestPoint(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point * result_point, int quality_threshold)
{ //Refactor this method by using getClosestPointSet and setting requested number of point to 1
	int cleared_array_size = size_of_array;
	LIDAR_data_point cleared_point_array[size_of_array];
	this->clearDataPointArrayFromUnqualifiedPoints(point_array, size_of_array, cleared_point_array, &cleared_array_size, quality_threshold);
	
	//std::cout <<"Initial size: " << size_of_array << " Cleaned array size: "<<cleared_array_size<<"\n";

	this->sortDataPointArrayInAscendingOrderByDistance(cleared_point_array, cleared_array_size); //tag cleared_point_array as sorted point array
	result_point->thetha = cleared_point_array[0].thetha;
	result_point->distance = cleared_point_array[0].distance;
	result_point->measurement_quality = cleared_point_array[0].measurement_quality;
}

void ObjectDetectionModule::getClosestPointSet(LIDAR_data_point point_array[], int size_of_array, LIDAR_data_point result_point_array[], int number_of_result_points_to_return, int quality_threshold)
{
	//cleanData
	int cleared_array_size = size_of_array;
	LIDAR_data_point cleared_point_array[size_of_array];
	this->clearDataPointArrayFromUnqualifiedPoints(point_array, size_of_array, cleared_point_array, &cleared_array_size, quality_threshold);
	
	std::cout <<"Initial size: " << size_of_array << " Cleaned array size: "<<cleared_array_size<<"\n";
	//sortData
	this->sortDataPointArrayInAscendingOrderByDistance(cleared_point_array, cleared_array_size);
	//return first X number of points
	for(int i=0; i<number_of_result_points_to_return; i++) {
		result_point_array[i].thetha = cleared_point_array[i].thetha;
		result_point_array[i].distance = cleared_point_array[i].distance;
		result_point_array[i].measurement_quality = cleared_point_array[i].measurement_quality;
	}
}