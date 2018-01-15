#include <iostream>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;


void initalize_data_collection_phase() {
   
}

int main(int argc, char* argv[]) {
   try {
      connection C("dbname = test user = ubuntu password = menohavepass \
      hostaddr = 127.0.0.1 port = 5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
         initalize_data_collection_phase();
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }
}

