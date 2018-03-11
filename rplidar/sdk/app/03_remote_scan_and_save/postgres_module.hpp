#ifndef POSTGRES_MODULE_H
#define POSTGRES_MODULE_H

#include <stdlib.h>
#include "LIDAR_data_structures.hpp"
#include <pqxx/pqxx>

class PostgresModule
{
	private:
		const char* db_name;
		const char* username;
		const char* password;
		const char* hostaddr;
		const char* port;
		const char* lidar_data_table_name;
		pqxx::connection* C;
	public:
		PostgresModule(const char*, const char*, const char*, const char*, const char*, const char*);
		~PostgresModule();
		bool connect();
		int getHighestDatasetNo();
		bool saveBatchScanData(LIDAR_batch_scan_data, int, bool);
		void disconnect();
};

#endif