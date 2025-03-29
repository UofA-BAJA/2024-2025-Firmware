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
#include <atomic>

#include <chrono>


#include "DataTypes.h"

namespace BajaWildcatRacing
{

    class DataStorage {
        public:
            DataStorage(const char* path);
            void startNewSession(const char* sessionName);
            void endCurrentSession();
            void execute(float timestamp);
            void end();
            int getData();
            void storeData(float data, DataTypes dataType);

        private:
            struct DataValues{
                int currentSessionID;
                double currentTimestamp;
                int dataType;
                double data;
            };

            struct DataTypeTable{
                DataTypes dataType;
                std::string name;
                std::string unit;
            };

            // Database handle for our sqlite database
            sqlite3* db;

            int currentSessionID = 0;
            float currentTimestamp = 0;
            int currentTimestampID = 0;

            ulong numDataInserts = 1;

            std::mutex insertBufferMutex;
            std::queue<DataValues> insertBuffer;

            // std::condition_variable insertCondition;
            // std::mutex cvMutex;

            std::vector<DataTypeTable> dataTypesInDB;

            std::thread updateDBThread;
            std::atomic<bool> running = true;

            void setupDatabase(const char* path);
            void setupDataTypes();

            void updateDatabase();
    };

}



#endif
