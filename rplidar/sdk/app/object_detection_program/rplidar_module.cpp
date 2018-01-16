#include <iostream>
#include "rplidar_module.hpp"


#include <stdio.h>
#include <stdlib.h>
#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef _WIN32
#include <Windows.h>
#define delay(x)   ::Sleep(x)
#else
#include <unistd.h>
static inline void delay(_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}
#endif

using namespace rp::standalone::rplidar;

bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;


    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("RPLidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}

void rplidar_start_scanning(RPlidarDriver * drv)
{
	drv->startMotor(); 	//start motor
    drv->startScan(); 	//start scan
}

void rplidar_stop_scanning(RPlidarDriver * drv)
{
	drv->stop(); 		//stop motor
    drv->stopMotor(); 	//stop scan
}

void rplidar_hardware_on_finish(RPlidarDriver * drv)
{
    if(drv) {
        std::cout << "Drv exists! Destroying it. \n";
        RPlidarDriver::DisposeDriver(drv); //drv is probably already disposed!!
    } else {
        std::cout << "Drv is already destroyed.\n";
    }
}

bool rplidar_hardware_initialization(RPlidarDriver * drv)
{
	bool is_initialization_succeed = true;

    u_result     op_result;

	const char * opt_com_path = NULL; 		//get com path as argument
    _u32         opt_com_baudrate = 115200; //get baudrate as argument

    printf("Ultra simple LIDAR data grabber for RPLIDAR.\n"
           "Version: " RPLIDAR_SDK_VERSION "\n");


    if (!opt_com_path) {
		#ifdef _WIN32
		        // use default com port
		        opt_com_path = "\\\\.\\com3";
		#else
		        opt_com_path = "/dev/ttyUSB0";
		#endif
    }

/*
    // create the driver instance
    drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
    
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        is_initialization_succeed = false;
        return is_initialization_succeed;
    }
*/

    // make connection...
    if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate))) {
        fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
            , opt_com_path);
        rplidar_hardware_on_finish(drv);
        is_initialization_succeed = false;
        return is_initialization_succeed;
    }

    rplidar_response_device_info_t devinfo;

	// retrieving the device info
    ////////////////////////////////////////
    op_result = drv->getDeviceInfo(devinfo);

    if (IS_FAIL(op_result)) {
        fprintf(stderr, "Error, cannot get device info.\n");
        rplidar_hardware_on_finish(drv);
        is_initialization_succeed = false;
        return is_initialization_succeed;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("RPLIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }

    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);



    // check health...
    if (!checkRPLIDARHealth(drv)) {
        rplidar_hardware_on_finish(drv);
        is_initialization_succeed = false;
        return is_initialization_succeed;
    }

    return is_initialization_succeed;
}

void rplidar_grab_batch_scan_data(RPlidarDriver * drv)
{
    u_result     op_result;

	rplidar_response_measurement_node_t nodes[360*2];
    size_t   count = _countof(nodes);
    op_result = drv->grabScanData(nodes, count);

    if (IS_OK(op_result)) {
        drv->ascendScanData(nodes, count);
        for (int pos = 0; pos < (int)count ; ++pos) {
            printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                (nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT) ?"S ":"  ", 
                (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f,
                nodes[pos].distance_q2/4.0f,
                nodes[pos].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
        }
    }
}


RplidarModule::RplidarModule(int lidar_no)
{
    this->lidar_ID = lidar_no;
    this->is_scanning = false;
    this->is_initialized = false;
    this->is_drv_set = false;
}

RplidarModule::~RplidarModule()
{
    std::cout << "Class destructor is executed! \n";
    this->disposeRplidar();
}

bool RplidarModule::initializeHardware() //No prob. here
{
    if(!(this->is_initialized)) {
    	bool is_rplidar_initialized = false;


        // create the driver instance
        this->drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
        
        if (!this->drv) {
            fprintf(stderr, "insufficent memory, exit\n");
            is_rplidar_initialized = false;
            return is_rplidar_initialized;
        }



    	is_rplidar_initialized = rplidar_hardware_initialization(this->drv); //is this->drv pass by value or reference???

        if (this->drv) {
            this->is_drv_set = true;
        } else {
            this->is_drv_set = false;
        }

        this->is_initialized = is_rplidar_initialized;
    	return is_rplidar_initialized;
    } else {
        std::cout << "Module is already initialized! \n";
        return false;
    }
}

void RplidarModule::startSystem()
{
    if(this->is_initialized) {
        std::cout << "RplidarModule "<< this->lidar_ID <<": starting the system! \n";

        RPlidarDriver * drv = this->drv;
        rplidar_start_scanning(drv);
        this->is_scanning = true;    
    } else {
        std::cout << "Module is not initialized! \n";
    }
}

void RplidarModule::grabBatchScanData() //this is the actual code that grabs data corresponding to while loop in ultra simple rplidar example program
{
    if(this->is_initialized) {
    	std::cout << "RplidarModule "<< this->lidar_ID <<": grabbing batch scan data. \n";
        rplidar_grab_batch_scan_data(this->drv);
    } else {
        std::cout << "Module is not initialized! \n";
    }
}

void RplidarModule::stopSystem() //user program MUST call this before program finishes. (including when ctrl+c is pressed)
{
    if(this->is_initialized) {
    	std::cout << "RplidarModule "<< this->lidar_ID <<": stopping the system!! \n";
    	rplidar_stop_scanning(this->drv);
    	this->is_scanning = false;
    } else {
        std::cout << "Module is not initialized! \n";
    }
}

void RplidarModule::disposeRplidar() //Am I also supposed to delete this class/object itself??
{
    if(this->is_initialized) {
        if(this->is_scanning) stopSystem(); //should i add the namespace thing?? - RplidarModule::stopSystem
        if(this->is_drv_set) {
            rplidar_hardware_on_finish(this->drv);
            this->is_drv_set = false;
        }
    } else {
        std::cout << "Module is not initialized! \n";
    }
}