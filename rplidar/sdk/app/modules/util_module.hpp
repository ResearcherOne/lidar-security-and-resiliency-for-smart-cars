#ifndef UTIL_MODULE_H
#define UTIL_MODULE_H

class UtilModule
{
	public:
		long long int getCurrentTimeMilliseconds();
		void setTimeFlag();
		int getMinutesSinceTimeFlag();
		int getSecondsSinceTimeFlag();
	private:
		long long int timeFlagInMs;
};

#endif