#include <stdio.h>
#include "books.h"

//Compiling the example on Ubuntu: g++ main.cpp -o main.o rplidar_module.h rplidar_module.cpp rplidar.h rplidar.cpp
int main()
{
	struct Books bookOne;
	initializeBook(&bookOne);
	printBook(&bookOne);
	return 0;
}