#include <iostream>
#include "rplidar_module.hpp"
#include "LIDAR_data_structures.hpp"
#include "postgres_module.hpp"
#include "util_module.hpp"
#include "../../../../cpp/06-writing-xbox-joystick-module/xbox_joystick.hpp"

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

RplidarModule* lidar;

int lidar_ID = 1;
bool is_exit_button_pressed = false;
bool is_data_collection_phase = false;
bool is_lidar_initialized = false;

void dispose_lidar() {
	if(is_lidar_initialized) {
		lidar->disposeRplidar();
		is_lidar_initialized = false;
	}
}

void toggle_data_collection() {
	if(!is_lidar_initialized) {
		printf("Initializing the LIDAR.");
		is_lidar_initialized = lidar->initializeHardware();
		printf("LIDAR state is set to: %d\n",is_lidar_initialized);
		if(is_lidar_initialized) {
			is_data_collection_phase = true;
			lidar->startSystem();
		}
	} else {
		is_data_collection_phase = !is_data_collection_phase;
		printf("Data collection state is set to: %d\n",is_data_collection_phase);
		if(is_data_collection_phase) {
			lidar->startSystem();
		} else {
			lidar->stopSystem();
		}
	}
}

void joystick_handler(xboxControllerEvents event) {
	printf("HANDLER IS CALLED \n");
	if(event == BACK_PRESS) {
		printf("BACK_PRESS \n");
	} else if (event == BACK_RELEASE) {
		printf("BACK_RELEASE \n");
		is_exit_button_pressed = true;
	} else if (event == START_PRESS) {
		printf("START_PRESS \n");
	} else if (event == START_RELEASE) {
		printf("START_RELEASE \n");
		toggle_data_collection();
	} else if (event == UPWARD_PRESS) {
		printf("UPWARD_PRESS \n");
	} else if (event == UPWARD_RELEASE) {
		printf("UPWARD_RELEASE \n");
	} else if (event == DOWNWARD_PRESS) {
		printf("DOWNWARD_PRESS \n");
	} else if (event == DOWNWARD_RELEASE) {
		printf("DOWNWARD_RELEASE \n");
	} else if (event == LEFTWARD_PRESS) {
		printf("LEFTWARD_PRESS \n");
	} else if (event == LEFTWARD_RELEASE) {
		printf("LEFTWARD_RELEASE \n");
	} else if (event == RIGHTWARD_PRESS) {
		printf("RIGHTWARD_PRESS \n");
	} else if (event == RIGHTWARD_RELEASE) {
		printf("RIGHTWARD_RELEASE \n");
	} else if (event == A_PRESS) {
		printf("Gas Pedal. \n");
	} else if (event == A_RELEASE) {
		printf("Gas Pedal Stop. \n");
	} else if (event == Y_PRESS) {
		printf("Y_PRESS. \n");
	} else if (event == Y_RELEASE) {
		printf("Y_RELEASE. \n");
	} else {
		printf("This button is not binded. \n");
	}
}

void sleepForMs(unsigned int milliseconds)
{
	usleep(milliseconds*1000);
}

int main()
{
	signal(SIGINT, ctrlc);

	initializeXboxJoystick("/dev/input/js0");
	setXboxEventHandler(joystick_handler);

	cout <<"This program will create rplidar_module with ID "<<lidar_ID<<" \n";
	lidar = new RplidarModule(lidar_ID);
	

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
	while(!ctrl_c_pressed && !is_exit_button_pressed) { //Change this to break while loop to number of grabbatchscandata.
		processJoystickEvents();
		if(is_data_collection_phase) {
			LIDAR_batch_scan_data lidar_batch_scan_data;
			lidar->grabBatchScanData(&lidar_batch_scan_data, lidar_ID, util.getCurrentTimeMilliseconds());//When two lidars are present, I will need to use threads in order to simultenously fetch data from them.
			postgres_module.saveBatchScanData(lidar_batch_scan_data, dataset_no, is_abnormal);
			total_collected_data_count = total_collected_data_count + lidar_batch_scan_data.scanned_data_count;
		}
	}
		cout<<"Counter: "<<counter<<" total point collected: "<<total_collected_data_count<<" dataset_no: "<<dataset_no<<"\n";

	postgres_module.disconnect();
	dispose_lidar();

	cout <<"Program is finished. \n";
	return 0;
}