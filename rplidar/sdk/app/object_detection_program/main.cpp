#include <iostream>
#include "rplidar_module.h"

using namespace std;

//Compiling the example on Ubuntu: g++ main.cpp -o main.o rplidar_module.h rplidar_module.cpp
int main()
{
	int tag_number = 1;
	cout <<"This program will create rplidar_module with tag number "<<tag_number<<" using constructor of rplidar_module class \n";
	
	RplidarModule stanley(tag_number);
	stanley.bark();
	return 0;
}