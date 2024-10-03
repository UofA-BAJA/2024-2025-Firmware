/* 
 *
 *
 * Author: Matthew Larson
 */

#include "DataStorage.h"


// Database handle for our sqlite database
sqlite3* db;


DataStorage::DataStorage(const char* path){

    setupDatabase(path);
}


int i = 0;

int DataStorage::getData(){
    std::cout << "Get Data Called, i: " << i << std::endl;
    return i;
}

void DataStorage::storeData(int data){
    i = data;
    std::cout << "Store Data Called, i: " << i << std::endl;
}

void DataStorage::setupDatabase(const char* path){

    int exit = 0;
    // If database does not already exist, it will be created.
    exit = sqlite3_open(path, &db);

    if (exit) { 
        std::cerr << "Error open DB: " << sqlite3_errmsg(db) << std::endl; 
        return;
    } 
    else
    {
        std::cout << "Opened Database Successfully!" << std::endl; 
    }
}