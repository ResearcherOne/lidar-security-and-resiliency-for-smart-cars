#include <iostream>
#include "rplidar_module.hpp"
#include "LIDAR_data_structures.hpp"
#include "postgres_module.hpp"
#include "util_module.hpp"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <pqxx/pqxx>//ELIMINATE THIS ASAP

#include <signal.h>
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

using namespace std;

void sleepForMs(unsigned int milliseconds)
{
	usleep(milliseconds*1000);
}

int main()
{
	signal(SIGINT, ctrlc);
	int lidar_ID = 1;
	cout <<"This program will create rplidar_module with ID "<<lidar_ID<<" \n";
	
	RplidarModule lidar(lidar_ID);

	lidar.initializeHardware("/dev/ttyUSB0");
	sleepForMs(2000);
	lidar.startSystem();

	UtilModule util;

	PostgresModule postgres_module("test", "researcher1", "menohavepass", "127.0.0.1", "5432", "rplidar_table_v2");
	bool is_connection_succeed = postgres_module.connect();
	if(!is_connection_succeed) {
		cout <<"Unable to connect to database, exiting.\n";
		exit (EXIT_FAILURE);
	}

	int dataset_no = 5;//(postgres_module.getHighestDatasetNo()+1);
	bool is_abnormal = false;
	
	int counter = 0;
	int total_collected_data_count = 0;
	cout<<"Counter: "<<counter<<" total point collected: "<<total_collected_data_count<<" dataset_no: "<<dataset_no<<"\n";
	while(!ctrl_c_pressed) { //Change this to break while loop to number of grabbatchscandata.
		//When two lidars are present, I will need to use threads in order to simultenously fetch data from them.
		LIDAR_batch_scan_data lidar_batch_scan_data;
		lidar.grabBatchScanData(&lidar_batch_scan_data, lidar_ID, util.getCurrentTimeMilliseconds());
		
		postgres_module.saveBatchScanData(lidar_batch_scan_data, dataset_no, is_abnormal);
		counter++;
		total_collected_data_count = total_collected_data_count + lidar_batch_scan_data.scanned_data_count;

		if(counter>=450) break;
	}
		cout<<"Counter: "<<counter<<" total point collected: "<<total_collected_data_count<<" dataset_no: "<<dataset_no<<"\n";

	postgres_module.disconnect();

	sleepForMs(2000);
	lidar.stopSystem();
	lidar.disposeRplidar();


	cout <<"Program is finished. \n";
	return 0;
}