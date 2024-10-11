/* 
 *
 *
 * Author: Matthew Larson
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <sqlite3.h>
#include <iostream>
#include <string.h>
#include <vector>


#include "DataTypes.h"

class DataStorage {
    public:
        DataStorage(const char* path);
        void startNewSession(const char* sessionName);
        void endCurrentSession();
        void execute(float timestamp);
        int getData();
        void storeData(float data, DataTypes dataType);
    private:
        int currentSessionID;
        float currentTimestamp;
        int currentTimestampID = 0;

        std::vector<DataTypes> dataTypesInDB;
        std::vector<std::string> dataTypeName;
        std::vector<std::string> dataTypeUnit;

        void setupDatabase(const char* path);
        void setupDataTypes();
};


#endif