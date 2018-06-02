/*
	05_timed_scan_save_log
	*removed robot base code
	*removed remote control code
	*removed primary/secondary node concept
	*changed program inputs
	*added program run time duration
*/
#include <iostream>
#include "../02_scan_and_save/rplidar_module.hpp"
#include "../03_remote_scan_and_save/LIDAR_data_structures.hpp"
#include "../02_scan_and_save/postgres_module.hpp"
#include "../modules/util_module.hpp"
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
LoggerModule* logger;
UtilModule util;

int max_usb_port_count = 4;

int lidar_ID = 0;
int runningDurationInMinutes = 0;
int programStartDelayInMinutes = 0;
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

bool isParametersValid(int argc, char* argv[]) //13 parameters
{
	if (argc != 14) {
        return 0;
    }

    return 1;
}

bool isRunningDurationOver(){
	int minutesPast = util.getMinutesSinceTimeFlag();
	return (minutesPast >= runningDurationInMinutes);
}

int main(int argc, char* argv[])
{
	if(!isParametersValid(argc, argv)) {
		std::cerr << "Usage: " << argv[0] << " <lidar_port> <lidar_id> <running_duration_in_minutes> <db_name> <db_username> <db_password> <db_ip> <db_port> <db_table_name> <db_dataset_no> <is_normal> <logger_export_file_path> <programStartDelayInMinutes>" << std::endl;
		return 0;
	}

	int dataset_no = 5;//(postgres_module.getHighestDatasetNo()+1);
	bool is_normal = true;

	lidar_com_port = argv[1];				//	/dev/ttyUSB4
	lidar_ID = atoi(argv[2]);				//	(int) 0,1,2,3...
	runningDurationInMinutes = atoi(argv[3]); 			//	(int) 1,2,3...
	const char* db_name = argv[4];			//	"test"
	const char* db_username = argv[5];		//	"researcher1"
	const char* db_password = argv[6];		//	"menohavepass"
	const char* db_ip = argv[7];			//	"127.0.0.1"
	const char* db_port = argv[8];			//	"5432"
	const char* db_table_name = argv[9];	//	"rplidar_table_v2"
	dataset_no = atoi(argv[10]);			//	5
	is_normal = atoi(argv[11]); 			//	(bool) 0 or 1
	logger = new LoggerModule(argv[12]);	// ./logger_export.log
	programStartDelayInMinutes = atoi(argv[13]); 			//	(int) 1,2,3...

	signal(SIGINT, ctrlc);

	lidar = new RplidarModule(lidar_ID);

	PostgresModule postgres_module(db_name, db_username, db_password, db_ip, db_port, db_table_name);
	bool is_connection_succeed = postgres_module.connect();
	if(!is_connection_succeed) {
		logger->log("Unable to connect to database, exiting.");
		exit (EXIT_FAILURE);
	}

	logger->log( "The program is delayed for "+toString(programStartDelayInMinutes)+" minutes.");
	sleepForMs(1000 * 60 * programStartDelayInMinutes);
	util.setTimeFlag();
	toggle_data_collection();
	
	int total_collected_data_count = 0;
	while(!ctrl_c_pressed && !is_exit_button_pressed && !isRunningDurationOver()) { //Change this to break while loop to number of grabbatchscandata.
		if(is_data_collection_phase) {
			LIDAR_batch_scan_data lidar_batch_scan_data;
			lidar->grabBatchScanData(&lidar_batch_scan_data, lidar_ID, util.getCurrentTimeMilliseconds());//When two lidars are present, I will need to use threads in order to simultenously fetch data from them.
			postgres_module.saveBatchScanData(lidar_batch_scan_data, dataset_no, is_normal);
			total_collected_data_count = total_collected_data_count + lidar_batch_scan_data.scanned_data_count;
		}
	}
	logger->log( "Total points collected: "+toString(total_collected_data_count)+" "
				+"lidar_com_port: "+argv[1]+" "
				+"lidar_ID: "+argv[2]+" "
				+"programDurationInMinutes: "+argv[3]+" "
				+"db_name: "+argv[4]+" "
				+"db_username: "+argv[5]+" "
				+"db_password: "+argv[6]+" "
				+"db_ip: "+argv[7]+" "
				+"db_port: "+argv[8]+" "
				+"db_table_name: "+argv[9]+" "
				+"dataset_no: "+argv[10]+" "
				+"is_normal: "+argv[11]+" "
				+"LoggerModule: "+argv[12]+" "
				+"programStartDelayInMinutes: "+argv[13]+" "
	);

	logger->log("Program is finished. \n");
	postgres_module.disconnect();
	lidar->stopSystem();
	sleepForMs(15000);
	dispose_lidar();

	return 0;
}