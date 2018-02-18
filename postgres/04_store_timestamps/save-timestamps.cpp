#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
   char * sql;
   
   try {
      connection C("dbname = test user = researcher1 password = menohavepass \
      hostaddr = 127.0.0.1 port = 5432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      /* Create SQL statement */
      sql = "INSERT INTO timestamp_table (id_timestamp, example_value) "  \
         "VALUES (to_timestamp(1395036000), 'Paul'); " \
         "INSERT INTO timestamp_table (id_timestamp,example_value) "  \
         "VALUES (to_timestamp(1395036001), 'Jason'); " \
         "INSERT INTO timestamp_table (id_timestamp,example_value) "  \
         "VALUES (to_timestamp(1395036002), 'Nick');";

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