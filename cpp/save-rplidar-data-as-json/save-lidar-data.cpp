#include <iostream>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <fstream>
#include <iostream>

using namespace std;

/*
 * 
 */
Json::Value getExampleRplidarJsonData() {
    Json::Value rplidarJsonData;
      
    Json::Value lidar_one;
    Json::Value batch_scan_data;
    Json::Value point_data_array;
    Json::Value single_point_data;
    
    single_point_data["timestamp"] = 123123123;
    single_point_data["thetha"] = 123;
    single_point_data["distance"] = 5;
    
    point_data_array.append(single_point_data);
    point_data_array.append(single_point_data);
    point_data_array.append(single_point_data);
  
    batch_scan_data["batch_timestamp"] = 123123123;
    batch_scan_data["scan_values"] = point_data_array;
    
    lidar_one.append(batch_scan_data);
    lidar_one.append(batch_scan_data);
    lidar_one.append(batch_scan_data);
    
    rplidarJsonData["lidar_one"] = lidar_one;
    rplidarJsonData["lidar_two"] = lidar_one;
    rplidarJsonData["lidar_three"] = lidar_one;
    
    return rplidarJsonData;
}

int main(int argc, char** argv) {
    Json::Value rplidarJsonData = getExampleRplidarJsonData();
    
    Json::FastWriter fast;
    Json::StyledWriter styled;
    string sFast = fast.write(rplidarJsonData);
    string sStyled = styled.write(rplidarJsonData);
    cout << "Fast:\n" << sFast << "Styled:\n" << sStyled;
    cout << "Styled stream:\n";
    Json::StyledStreamWriter styledStream;
    styledStream.write(cout, rplidarJsonData);

    ofstream myfile;
    myfile.open ("rplidar_scan_data.json");
    styledStream.write(myfile, rplidarJsonData);
    myfile.close();
    
    return 1;
}

