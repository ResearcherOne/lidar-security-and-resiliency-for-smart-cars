#include <iostream>
#include <pqxx/pqxx>
#include <sys/timeb.h> 
#include <unistd.h>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
   const char * sql;
   
   try {
      connection C("dbname = test user = researcher1 password = menohavepass \
      hostaddr = 127.0.0.1 port = 5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      for(int i=0; i<5; i++) {
         
         struct timeb timer_msec;
         long long int timestamp_msec; /* timestamp in millisecond. */
         if (!ftime(&timer_msec)) {
            timestamp_msec = ((long long int) timer_msec.time) * 1000ll + 
                        (long long int) timer_msec.millitm;
         } else {
            timestamp_msec = -1;
         }
         printf("%lld milliseconds since epoch\n", timestamp_msec);


         std::stringstream ss;
         ss << "INSERT INTO timestamp_table_v2 (id_timestamp, example_value) VALUES ("<<timestamp_msec<<", 'now!'); ";
         const std::string tmp = ss.str();
         sql = tmp.c_str();

         work W(C);
         W.exec( sql );
         W.commit();
         cout << "Records created successfully" << endl;

         usleep(1000*1000);
      }

      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}