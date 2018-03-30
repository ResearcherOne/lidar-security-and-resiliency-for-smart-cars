#include <iostream>
#include "postgres_module.hpp"
#include "LIDAR_data_structures.hpp"
#include <pqxx/pqxx>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace pqxx;

PostgresModule::PostgresModule(const char* db_name, const char* username, const char* password, const char* hostaddr, const char* port, const char* lidar_data_table_name)
{
	this->db_name = db_name;
	this->username = username;
	this->password = password;
	this->hostaddr = hostaddr;
	this->port = port;
	this->lidar_data_table_name = lidar_data_table_name;
}

PostgresModule::~PostgresModule()
{
	this->disconnect();
}

bool PostgresModule::connect()
{
	try {

      std::stringstream ss;
      ss << "dbname = "<<this->db_name<<" user = "<<this->username<<" password = "<<this->password<<" hostaddr = "<<this->hostaddr<<" port = "<<this->port<<"";
      const std::string tmp = ss.str();

      this->C = new connection(tmp.c_str());
      if ((this->C)->is_open()) {
         cout << "Opened Database Successfully: " << (this->C)->dbname() << endl;
         return true;
      } else {
         cout << "Can't open database!" << endl;
         return false;
      }

   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return false;
   }
}

int PostgresModule::getHighestDatasetNo() //For every program execution, I'll simply change return value and recompile the program
{
	return 0;
}
bool PostgresModule::saveBatchScanData(LIDAR_batch_scan_data lidar_batch_scan_data, int dataset_no, bool is_normal)
{
   long long int batch_timestamp = lidar_batch_scan_data.timestamp_msec;
   int data_point_count = lidar_batch_scan_data.scanned_data_count;
   int lidar_ID = lidar_batch_scan_data.lidar_ID;
   LIDAR_data_point* lidar_data_point_array = lidar_batch_scan_data.LIDAR_data_point_array;

   std::stringstream ss;
   
   for(int i=0; i<data_point_count; i++) {
      LIDAR_data_point current_point = lidar_data_point_array[i];
      if(is_normal)
         ss << "INSERT INTO "<<this->lidar_data_table_name<<" (dataset_no, lidar_no, timestamp_ms, thetha, distance, quality, normal) VALUES ("<<dataset_no<<","<<lidar_ID<<","<<batch_timestamp<<","<<current_point.thetha<<","<<current_point.distance<<","<<current_point.measurement_quality<<",true); "; //quick and dirty solution (also quite silly, sry).
      else
         ss << "INSERT INTO "<<this->lidar_data_table_name<<" (dataset_no, lidar_no, timestamp_ms, thetha, distance, quality, normal) VALUES ("<<dataset_no<<","<<lidar_ID<<","<<batch_timestamp<<","<<current_point.thetha<<","<<current_point.distance<<","<<current_point.measurement_quality<<",false); ";
   }

   const std::string tmp = ss.str();

   work W(*(this->C));
   W.exec(tmp.c_str());
   W.commit();

	return false;
}
void PostgresModule::disconnect()
{
   (this->C)->disconnect ();
}