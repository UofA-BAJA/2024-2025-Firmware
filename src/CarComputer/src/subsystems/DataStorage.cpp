/* 
 *
 *
 * Author: Matthew Larson
 */

#include "DataStorage.h"


// DataStorage::DataStorage(const char* path){
//     // Create a connection to our sqlite database stored 
//     // on the SSD connected to the PI

//     sqlite3* db;

//     int exit = 0;


// }


int i = 0;

int DataStorage::getData(){
    std::cout << "Get Data Called, i: " << i << std::endl;
    return i;
}

void DataStorage::storeData(int data){
    i = data;
    std::cout << "Store Data Called, i: " << i << std::endl;
}