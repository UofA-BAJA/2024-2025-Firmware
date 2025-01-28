#include "DataStorage.h"

namespace BajaWildcatRacing
{

    // Database handle for our sqlite database
    sqlite3* db;


    DataStorage::DataStorage(const char* path){
        setupDatabase(path);

        setupDataTypes();

        updateDBThread = std::thread(&DataStorage::updateDatabase, this);

    }


    void DataStorage::updateDatabase(){
        const char* insertData = "INSERT OR IGNORE INTO Data (SessionID, Timestamp, DataTypeID, Value) "
                                "VALUES (?, ?, ?, ?)";

        char* messageError;
        int exit;

        while(true){
            // insertCondition.wait(conditionalLock, [this] { return insertBuffer.size() > 20; });

            std::unique_lock<std::mutex> lock(insertBufferMutex);

            std::queue<dataValues> localInsertBuffer;
            std::swap(localInsertBuffer, insertBuffer);
            lock.unlock();


            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &messageError);


            while(!localInsertBuffer.empty()){

                dataValues data = localInsertBuffer.front();
                localInsertBuffer.pop();


                int exit = 0;

                sqlite3_stmt *statement;

                exit = sqlite3_prepare_v2(db, insertData, -1, &statement, nullptr);

                if(exit){
                    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
                    return;
                }

                // Bind values to parameters
                sqlite3_bind_int(statement, 1, data.currentSessionID);
                sqlite3_bind_double(statement, 2, data.currentTimestamp);
                sqlite3_bind_int(statement, 3, data.dataType);
                sqlite3_bind_double(statement, 4, data.data);

                // std::cout << "SessionID: " << data.currentSessionID << " DataType: " << data.dataType << " Timestamp: " << data.currentTimestamp << std::endl;

                exit = sqlite3_step(statement);

                if(exit != SQLITE_DONE){
                    std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_finalize(statement);
            }

            sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &messageError);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    }




    const char* insertSession = "INSERT INTO Session (Name) VALUES (?)";

    void DataStorage::startNewSession(const char* sessionName){

        int exit = 0;

        sqlite3_stmt *statement;

        exit = sqlite3_prepare_v2(db, insertSession, -1, &statement, nullptr);

        if(exit){
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Bind values to parameters
        sqlite3_bind_text(statement, 1, sessionName, -1, SQLITE_STATIC);

        exit = sqlite3_step(statement);

        if(exit != SQLITE_DONE){
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }

        currentSessionID = sqlite3_last_insert_rowid(db);

        sqlite3_finalize(statement);
    }




    const char* updateEndTime = "UPDATE Session SET EndTime = CURRENT_TIMESTAMP WHERE SessionID = ?;";

    void DataStorage::endCurrentSession(){
        
        int exit = 0;

        sqlite3_stmt *statement;

        exit = sqlite3_prepare_v2(db, updateEndTime, -1, &statement, nullptr);

            if(exit){
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Bind values to parameters
        sqlite3_bind_int(statement, 1, currentSessionID);

        exit = sqlite3_step(statement);

        if(exit != SQLITE_DONE){
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(statement);
    }


    void DataStorage::execute(float timestamp){

        currentTimestamp = timestamp;
    }


    int i = 0;

    int DataStorage::getData(){
        std::cout << "Get Data Called, i: " << i << std::endl;
        return i;
    }


    /*
    *  Method:  storeData
    *
    *  Purpose: Inserts a given floating point value to store in association with a data
    *           type and timestamp. 
    *
    *  Pre-Condition:  [Any non-obvious conditions that must exist
    *              or be true before we can expect this method to
    *              function correctly.]
    *
    *  Post-Condition: The data given will be stored in the database in the data table;
    *                  The data is stored in association with the given data type and the
    *                  current car timestamp (time since car program started);
    *
    *  Parameters:
    *          data -- the floating point data value to enter into the database
    *          dataType -- the type of data to enter. ie. IMU X Rotation
    *
    *  Returns: None
    *
    */

    void DataStorage::storeData(float data, DataTypes dataType){

        dataValues dataToStore;
        dataToStore.currentSessionID = currentSessionID;
        dataToStore.currentTimestamp = currentTimestamp;
        dataToStore.dataType = dataType;
        dataToStore.data = data;


        numDataInserts++;
        std::lock_guard<std::mutex> lock (insertBufferMutex);
        insertBuffer.push(dataToStore);
    }





    void DataStorage::setupDatabase(const char* path){

        std::string fullPath = std::string(path) + "/testdb.db";

        int exit = 0;
        // If database does not already exist, it will be created.
        exit = sqlite3_open(fullPath.c_str(), &db);

        if (exit) { 
            std::cerr << "Error open DB: " << sqlite3_errmsg(db) << std::endl; 
            return;
        } 
        else
        {
            // std::cout << "Opened Database Successfully!" << std::endl; 
        }
        
        char* messageError;

        const char* threadingMode = "PRAGMA compile_options;";
        sqlite3_exec(db, threadingMode, NULL, 0, &messageError);

        // We have to enable the foreign key constraint. This ensures we are setting 
        // all of the tables up correctly and will have the appropriate data.
        const char* enableForeignKeys = "PRAGMA foreign_keys = ON;";

        exit = sqlite3_exec(db, enableForeignKeys, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error Enabling Foreign Keys: " << messageError << std::endl;
            sqlite3_free(messageError);
        }

        const char* setJournalMode = "PRAGMA journal_mode = WAL;";

        exit = sqlite3_exec(db, setJournalMode, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error setting journal_mode: " << messageError << std::endl;
            sqlite3_free(messageError);
        }

        const char* setSynchronous = "PRAGMA synchronous = NORMAL;";

        exit = sqlite3_exec(db, setSynchronous, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error setting synchronous: " << messageError << std::endl;
            sqlite3_free(messageError);
        }


        const char* createSessionTable = "CREATE TABLE IF NOT EXISTS Session("
                        "SessionID INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "StartTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                        "EndTime TIMESTAMP,"
                        "Name TEXT        NOT NULL)";

        exit = sqlite3_exec(db, createSessionTable, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error Create Table: " << messageError << std::endl;
            sqlite3_free(messageError);
        }
        else{
            // std::cout << "Session table created successfully" << std::endl;
        }

        // Create DataTypes Table
        // The DataTypeID is the same as the DataTypes enum value.
        const char* createDataTypeTable = "CREATE TABLE IF NOT EXISTS DataType("
                        "DataTypeID INTEGER PRIMARY KEY, "
                        "DataTypeName TEXT,"
                        "DataUnit TEXT)";

        exit = sqlite3_exec(db, createDataTypeTable, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error Create Table: " << messageError << std::endl;
            sqlite3_free(messageError);
        }
        else{
            // std::cout << "DataType table created successfully" << std::endl;
        }

        // Create Data Table
        const char* dataCreateTable = "CREATE TABLE IF NOT EXISTS Data("
                                    "SessionID INTEGER,"
                                    "Timestamp REAL,"
                                    "DataTypeID INTEGER,"
                                    "Value REAL,"
                                    "PRIMARY KEY (SessionID, Timestamp, DataTypeID),"
                                    "FOREIGN KEY (SessionID) REFERENCES Session(SessionID) ON DELETE CASCADE,"
                                    "FOREIGN KEY (DataTypeID) REFERENCES DataType(DataTypeID) ON DELETE NO ACTION"
                                    ");";


        exit = sqlite3_exec(db, dataCreateTable, NULL, 0, &messageError);

        if(exit != SQLITE_OK){
            std::cerr << "Error Create Table: " << messageError << std::endl;
            sqlite3_free(messageError);
        }
        else{
            // std::cout << "Data table created successfully" << std::endl;
        }
    }


    void DataStorage::setupDataTypes(){

        // Extremely important that all 3 lists have the same size and that each index of each list
        // corresponds to the same data type! If it doesn't things will break.
        // There's definitely a better way to do this (if somebody wants to fix pls do!)

        dataTypesInDB.push_back(DataTypes::IMU_ROTATION_X);
        dataTypeName.push_back("IMU ROTATION X");
        dataTypeUnit.push_back("deg");

        dataTypesInDB.push_back(DataTypes::IMU_ROTATION_Y);
        dataTypeName.push_back("IMU ROTATION Y");
        dataTypeUnit.push_back("deg");

        dataTypesInDB.push_back(DataTypes::IMU_ROTATION_Z);
        dataTypeName.push_back("IMU ROTATION Z");
        dataTypeUnit.push_back("deg");

        dataTypesInDB.push_back(DataTypes::IMU_ACCELERATION_X);
        dataTypeName.push_back("IMU ACCELERATION X");
        dataTypeUnit.push_back("m/s^2");

        dataTypesInDB.push_back(DataTypes::IMU_ACCELERATION_Y);
        dataTypeName.push_back("IMU ACCELERATION Y");
        dataTypeUnit.push_back("m/s^2");

        dataTypesInDB.push_back(DataTypes::IMU_ACCELERATION_Z);
        dataTypeName.push_back("IMU ACCELERATION Z");
        dataTypeUnit.push_back("m/s^2");

        dataTypesInDB.push_back(DataTypes::CAR_SPEED);
        dataTypeName.push_back("CAR SPEED");
        dataTypeUnit.push_back("m/s");

        /* I'm not entirely sure how the ON CONFLICT works, but we need to do this if we want to update the data types table. 
        * The reason why is that all of the data in the data table has a foreign key that references this table, so if it's
        * inserted or replaced every time it takes a ton of time to fix if the database is large (which it will be)
        *
        */ 

        const char* insertDataType = "INSERT INTO DataType (DataTypeID, DataTypeName, DataUnit) VALUES (?, ?, ?) "
                                    "ON CONFLICT(DataTypeID) DO UPDATE SET DataTypeName=excluded.DataTypeName, DataUnit=excluded.DataUnit";

        for(int i = 0; i < dataTypesInDB.size(); i++){
            int exit = 0;

            sqlite3_stmt *statement;

            exit = sqlite3_prepare_v2(db, insertDataType, -1, &statement, nullptr);


            if(exit){
                std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // Bind values to parameters
            sqlite3_bind_double(statement, 1, dataTypesInDB[i]);
            sqlite3_bind_text(statement, 2, dataTypeName[i].c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, dataTypeUnit[i].c_str(), -1, SQLITE_STATIC);


            exit = sqlite3_step(statement);

            if(exit != SQLITE_DONE){
                std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_finalize(statement);
        }
    }



        // enum DataRetrieval{
        //     RETRIEVE_ALL,
        //     RETRIEVE
        // }

}
