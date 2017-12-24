#include <iostream>
#include <pqxx/pqxx> 
#include <sys/timeb.h> /* ftime, timeb (for timestamp in millisecond) */
#include <sstream>
#include <string>

using namespace std;
using namespace pqxx;


long long int get_current_time_in_ms() {
   struct timeb timer_msec;
     long long int timestamp_msec; /* timestamp in millisecond. */
     if (!ftime(&timer_msec)) {
      timestamp_msec = ((long long int) timer_msec.time) * 1000ll + 
                     (long long int) timer_msec.millitm;
     } else {
      timestamp_msec = -1;
     }
   printf("%lld milliseconds since epoch\n", timestamp_msec);
   return timestamp_msec;
}

int main(int argc, char* argv[]) {
   char * sql;
   
   try {
      connection C("dbname = test user = ubuntu password = menohavepass \
      hostaddr = 127.0.0.1 port = 5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      long long int current_timestamp = get_current_time_in_ms();
      /* Create SQL statement */
      stringstream myString;
      myString << "INSERT INTO timestamp_table_2 (timestamp_ms) "  \
         "VALUES ("<<current_timestamp<<"); " \
         "INSERT INTO timestamp_table_2 (timestamp_ms) "  \
         "VALUES ("<<(current_timestamp+1)<<");";

      std::string s = myString.str();
      sql = s.str();

      /* Create a transactional object. */
      work W(C);
      
      /* Execute SQL query */
      W.exec( sql );
      W.commit();
      cout << "Records created successfully" << endl;
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}
