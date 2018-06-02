#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "../util_module.hpp"

void sleepForMs(unsigned int milliseconds)
{
	usleep(milliseconds*1000);
}

void testGetMinutesSinceTimeFlag()
{
	bool isSuccess = true;

	UtilModule util;
	util.setTimeFlag();
	
	isSuccess = isSuccess&&(util.getMinutesSinceTimeFlag()==0);
	sleepForMs(65000);
	isSuccess = isSuccess&&(util.getMinutesSinceTimeFlag()==1);
	sleepForMs(60000);
	isSuccess = isSuccess&&(util.getMinutesSinceTimeFlag()==2);

    if(!isSuccess) printf("X - testGetMinutesSinceTimeFlag failed.\n");
    else printf("testGetMinutesSinceTimeFlag succeeded.\n");
}

void testGetSecondsSinceTimeFlag()
{
	bool isSuccess = true;

	UtilModule util;
	util.setTimeFlag();
	
	isSuccess = isSuccess&&(util.getSecondsSinceTimeFlag()==0);
	sleepForMs(2000);
	isSuccess = isSuccess&&(util.getSecondsSinceTimeFlag()==2);

    if(!isSuccess) printf("X - testGetSecondsSinceTimeFlag failed.\n");
    else printf("testGetSecondsSinceTimeFlag succeeded.\n");
}

int main()
{
	testGetSecondsSinceTimeFlag();
	testGetMinutesSinceTimeFlag();
}
