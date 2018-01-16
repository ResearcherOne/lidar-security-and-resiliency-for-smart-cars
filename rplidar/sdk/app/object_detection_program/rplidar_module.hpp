#ifndef RPLIDAR_MODULE_H
#define RPLIDAR_MODULE_H

#include <stdlib.h>
#include "rplidar.h"

class RplidarModule
{
	private:
		bool is_scanning;
		bool is_initialized;
		bool is_drv_set; //This is just quick work around! How to check properly whether drv object exist or not in C++??
		int lidar_ID;
	public:
		rp::standalone::rplidar::RPlidarDriver * drv;
		RplidarModule(int);
		~RplidarModule(); //When is this function executed??
		bool initializeHardware(); //Instead of this, use constructor. Set arguments taken from user and default values of variables in the constructor too.
		void startSystem();
		void grabBatchScanData(); //Create LIDAR_DATA_STRUCTURES.h Creating my own header will let me use other LIDAR with minimal change to my code.
		void stopSystem();
		void disposeRplidar();
};

#endif