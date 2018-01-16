#include <iostream>
#include "rplidar_module.hpp"


/*
	Although user program is written in C, it is somehow necessary to name the program in cpp because RPLIDAR sdk (as far as I understand)
	make use of some C++ libraries. But when I use classes, compiler does not recognize them. So user program is written in C but compiled
	under the name of C++ program.
*/

int main()
{
	int lidar_ID = 4;
	std::cout <<"This program will create rplidar_module with ID "<<lidar_ID<<" \n";
	
	struct RplidarModule lidar;

	RplidarModule_initializeSystem(&lidar, lidar_ID);
	RplidarModule_startSystem(&lidar);
	return 0;
}