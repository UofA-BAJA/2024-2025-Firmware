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
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>


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
        struct dataValues{
            int currentSessionID;
            double currentTimestamp;
            int dataType;
            double data;
        };

        int currentSessionID;
        float currentTimestamp;
        int currentTimestampID = 0;

        ulong numDataInserts = 1;

        std::mutex insertBufferMutex;
        std::queue<dataValues> insertBuffer;

        // std::condition_variable insertCondition;
        // std::mutex cvMutex;


        std::vector<DataTypes> dataTypesInDB;
        std::vector<std::string> dataTypeName;
        std::vector<std::string> dataTypeUnit;

        std::thread updateDBThread;

        void setupDatabase(const char* path);
        void setupDataTypes();

        void updateDatabase();
};



#endif