#include "util_module.hpp"
#include <sys/timeb.h> 

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