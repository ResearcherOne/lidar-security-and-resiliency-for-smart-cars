#ifndef RPLIDAR_MODULE_H
#define RPLIDAR_MODULE_H

class RplidarModule
{
	private:
		int lidar_ID;
	public:
		bool initializeSystem(int);
		void startSystem();
		void stopSystem();
		void grabBatchScanData(); //Create LIDAR_DATA_STRUCTURES.h Creating my own header will let me use other LIDAR with minimal change to my code.
};

#endif