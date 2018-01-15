#include <iostream>
#include "rplidar_module.h"

using namespace std;


RplidarModule::RplidarModule(int tag_no)
{
	tag_number = tag_no;
}

void RplidarModule::bark()
{
	cout << "RplidarModule "<< tag_number <<": Get the hell out of my zone you little human!! \n";
}