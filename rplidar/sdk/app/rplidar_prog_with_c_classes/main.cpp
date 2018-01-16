#include <stdio.h>
#include <iostream>
#include "books.h"

//It is OK to name books.cpp and main.cpp your C program. This can be necessary to do so as rplidar may contain library from C++ library.

int main()
{
	struct Books bookOne;
	initializeBook(&bookOne);
	printBook(&bookOne);
	return 0;
}