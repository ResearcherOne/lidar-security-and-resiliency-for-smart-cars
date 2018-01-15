#include <iostream>
#include "rplidar_module.h"

using namespace std;

//Compiling the example on Ubuntu: g++ main.cpp -o main.o rplidar_module.h rplidar_module.cpp
int main()
{
	int lidar_ID = 5;
	cout <<"This program will create rplidar_module with ID "<<lidar_ID<<" \n";
	
	RplidarModule lidar;
	lidar.initializeSystem(lidar_ID);
	lidar.startSystem();
	return 0;
}