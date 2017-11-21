#include <iostream>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <fstream>

using namespace std;

/*
 * 
 */
void output(const Json::Value & value) {
    // querying the json object is very simple
    std::cout << value["hello"];
    std::cout << value["number"];
    std::cout << value["array"][0] << value["array"][1];
    std::cout << value["object"]["hello"];
}

int main(int argc, char** argv) {
    // define jsoncpp value
    Json::Value fromScratch;
    Json::Value array;
    // array values
    array.append("hello");
    array.append("world");
    
    fromScratch["hello"] = "world";
    fromScratch["number"] = 2;
    fromScratch["array"] = array;
    fromScratch["object"]["hello"] = "world";

    output(fromScratch);


    return 1;
}

