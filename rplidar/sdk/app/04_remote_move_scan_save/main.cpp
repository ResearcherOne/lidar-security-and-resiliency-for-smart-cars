#include <iostream>
#include "../02_scan_and_save/rplidar_module.hpp"
#include "../03_remote_scan_and_save/LIDAR_data_structures.hpp"
#include "../03_remote_scan_and_save/postgres_module.hpp"
#include "../02_scan_and_save/util_module.hpp"
#include "../../../../cpp/06-writing-xbox-joystick-module/xbox_joystick.hpp"
#include "robot_base.hpp"
#include "../../../../cpp/09-logger-module/logger_module.hpp"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#include <pqxx/pqxx>//ELIMINATE THIS ASAP

#include <signal.h>
#include <sstream>

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

using namespace std;

RplidarModule* lidar;
RobotBase* robot_base;
LoggerModule* logger;

bool isPrimaryNode = true;
int robot_base_USB_no = 1;
int max_usb_port_count = 4;

int lidar_ID = 0;
bool is_exit_button_pressed = false;
bool is_data_collection_phase = false;
bool is_lidar_initialized = false;
const char* lidar_com_port;

template <typename T>
string toString(T val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

void sleepForMs(unsigned int milliseconds)
{
	usleep(milliseconds*1000);
}

void dispose_lidar() {
	if(is_lidar_initialized) {
		lidar->disposeRplidar();
		is_lidar_initialized = false;
	}
}

void toggle_data_collection() {
	if(!is_lidar_initialized) {
		printf("Initializing the LIDAR.");
		is_lidar_initialized = lidar->initializeHardware(lidar_com_port);
		printf("LIDAR state is set to: %d\n",is_lidar_initialized);
		if(is_lidar_initialized) {
			is_data_collection_phase = true;
			lidar->startSystem();
			logger->log("Data collection is started.");
		}
	} else {
		is_data_collection_phase = !is_data_collection_phase;
		if(is_data_collection_phase) {
			lidar->startSystem();
			logger->log("Data collection is started.");
		} else {
			lidar->stopSystem();
			logger->log("Data collection is stopped.");
		}
	}
}

void joystick_handler(xboxControllerEvents event) {
	if(event == BACK_PRESS) {
	} else if (event == BACK_RELEASE) {
		is_exit_button_pressed = true;
	} else if (event == START_PRESS) {
	} else if (event == START_RELEASE) {
		toggle_data_collection();
	} else if (event == UPWARD_PRESS) {
		if(isPrimaryNode) robot_base->forward();
	} else if (event == UPWARD_RELEASE) {
	} else if (event == DOWNWARD_PRESS) {
		if(isPrimaryNode) robot_base->backward();
	} else if (event == DOWNWARD_RELEASE) {
	} else if (event == LEFTWARD_PRESS) {
		if(isPrimaryNode) robot_base->left();
	} else if (event == LEFTWARD_RELEASE) {
	} else if (event == RIGHTWARD_PRESS) {
		if(isPrimaryNode) robot_base->right();
	} else if (event == RIGHTWARD_RELEASE) {
	} else if (event == A_PRESS) {
		if(isPrimaryNode) robot_base->move();
	} else if (event == A_RELEASE) {
		if(isPrimaryNode) robot_base->stop();
	} else if (event == Y_PRESS) {
	} else if (event == Y_RELEASE) {
	} else if (event == X_PRESS) {
		if(isPrimaryNode) robot_base->makeBuzzerNoise();
	} else if (event == X_RELEASE) {
	} else if (event == RIGHT_JOYSTICK_CLICK_PRESS) {
		if(isPrimaryNode) {
			if(robot_base_USB_no>=max_usb_port_count) robot_base_USB_no = 0;
			else robot_base_USB_no++;
			robot_base->setUSB(robot_base_USB_no);
			printf("Robot Base USB Port Set To %d \n",robot_base_USB_no);
		}
	}
	else {
		printf("This button is not binded. \n");
	}
}

bool isParametersValid(int argc, char* argv[]) //13 parameters
{
	if (argc != 14) {
        return 0;
    }

    return 1;
}

int main(int argc, char* argv[])
{
	if(!isParametersValid(argc, argv)) {
		std::cerr << "Usage: " << argv[0] << " <lidar_port> <lidar_id> <is_primary_node> <robot_base_usb_no> <db_name> <db_username> <db_password> <db_ip> <db_port> <db_table_name> <db_dataset_no> <is_abnormal_dataset> <logger_export_file_path>" << std::endl;
		return 0;
	}

	int dataset_no = 5;//(postgres_module.getHighestDatasetNo()+1);
	bool is_abnormal = false;

	lidar_com_port = argv[1];				//	/dev/ttyUSB4
	lidar_ID = atoi(argv[2]);				//	(int) 0,1,2,3...
	isPrimaryNode = atoi(argv[3]); 			//	(bool) 0 or 1
	robot_base_USB_no = atoi(argv[4]); 		//	(int) 0,2,3,4...
	const char* db_name = argv[5];			//	"test"
	const char* db_username = argv[6];		//	"researcher1"
	const char* db_password = argv[7];		//	"menohavepass"
	const char* db_ip = argv[8];			//	"127.0.0.1"
	const char* db_port = argv[9];			//	"5432"
	const char* db_table_name = argv[10];	//	"rplidar_table_v2"
	dataset_no = atoi(argv[11]);			//	5
	is_abnormal = atoi(argv[12]); 			//	(bool) 0 or 1
	logger = new LoggerModule(argv[13]);			// ./logger_export.log

	signal(SIGINT, ctrlc);

	initializeXboxJoystick("/dev/input/js0");
	setXboxEventHandler(joystick_handler);

	lidar = new RplidarModule(lidar_ID);

	if(isPrimaryNode) {
		robot_base = new RobotBase();
		robot_base->setUSB(robot_base_USB_no);
	}

	UtilModule util;
	PostgresModule postgres_module(db_name, db_username, db_password, db_ip, db_port, db_table_name);
	bool is_connection_succeed = postgres_module.connect();
	if(!is_connection_succeed) {
		logger->log("Unable to connect to database, exiting.");
		exit (EXIT_FAILURE);
	}

	
	int total_collected_data_count = 0;
	while(!ctrl_c_pressed && !is_exit_button_pressed) { //Change this to break while loop to number of grabbatchscandata.
		processJoystickEvents();
		if(is_data_collection_phase) {
			LIDAR_batch_scan_data lidar_batch_scan_data;
			lidar->grabBatchScanData(&lidar_batch_scan_data, lidar_ID, util.getCurrentTimeMilliseconds());//When two lidars are present, I will need to use threads in order to simultenously fetch data from them.
			postgres_module.saveBatchScanData(lidar_batch_scan_data, dataset_no, is_abnormal);
			total_collected_data_count = total_collected_data_count + lidar_batch_scan_data.scanned_data_count;
		}
	}
	logger->log(	 "Total points collected: "+toString(total_collected_data_count)+" "
				+"lidar_com_port: "+argv[1]+" "
				+"lidar_ID: "+argv[2]+" "
				+"isPrimaryNode: "+argv[3]+" "
				+"robot_base_USB_no: "+argv[4]+" "
				+"db_name: "+argv[5]+" "
				+"db_username: "+argv[6]+" "
				+"db_password: "+argv[7]+" "
				+"db_ip: "+argv[8]+" "
				+"db_port: "+argv[9]+" "
				+"db_table_name: "+argv[10]+" "
				+"dataset_no: "+argv[11]+" "
				+"is_abnormal: "+argv[12]+" "
				+"LoggerModule: "+argv[13]+" "
	);

	postgres_module.disconnect();
	lidar->stopSystem();
	sleepForMs(15000);
	dispose_lidar();

	logger->log("Program is finished. \n");
	return 0;
}