#include <sys/timeb.h> 
#include <stdio.h>
#include "util_module.hpp"

long long int UtilModule::getCurrentTimeMilliseconds()
{
	struct timeb timer_msec;
	long long int timestamp_msec; /* timestamp in millisecond. */
	if (!ftime(&timer_msec)) {
		timestamp_msec = ((long long int) timer_msec.time) * 1000ll + 
	            (long long int) timer_msec.millitm;
	} else {
		timestamp_msec = -1;
	}
	return timestamp_msec;
}

void UtilModule::setTimeFlag()
{
	this->timeFlagInMs = this->getCurrentTimeMilliseconds();
}

int UtilModule::getSecondsSinceTimeFlag()
{
	long long int currentTimestamp = this->getCurrentTimeMilliseconds();
	long long int timeDifferenceInMs = currentTimestamp - this->timeFlagInMs;
	int timeDifferenceInSeconds = (int)(timeDifferenceInMs / 1000);
	return timeDifferenceInSeconds;
}

int UtilModule::getMinutesSinceTimeFlag()
{
	return (this->getSecondsSinceTimeFlag() / 60);
}