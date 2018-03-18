#ifndef RPLIDAR_MODULE_H
#define RPLIDAR_MODULE_H

#include <stdlib.h>
#include "rplidar.h"
#include "LIDAR_data_structures.hpp"

class RplidarModule
{
	private:
		bool is_scanning;
		bool is_initialized;
		bool is_drv_set; //This is just quick work around! How to check properly whether drv object exist or not in C++??
		int lidar_ID;
		//void rplidar_batch_scan_to_standard_LIDAR_batch_form(rplidar_response_measurement_node_t rplidar_batch_scan_data, LIDAR_batch_scan_data * LIDAR_batch_scan, int scan_point_count);
		//shall i write every function in rplidar_module that is not public as private?
	public:
		rp::standalone::rplidar::RPlidarDriver * drv;
		RplidarModule(int);
		~RplidarModule(); //When is this function executed??
		bool initializeHardware(const char * opt_com_path); //Instead of this, use constructor. Set arguments taken from user and default values of variables in the constructor too.
		void startSystem();
		void grabBatchScanData(LIDAR_batch_scan_data * lidar_batch_scan_data, int LIDAR_ID, long long int batch_timestamp); //Create LIDAR_DATA_STRUCTURES.h Creating my own header will let me use other LIDAR with minimal change to my code.
		void stopSystem();
		void disposeRplidar();
};

#endif