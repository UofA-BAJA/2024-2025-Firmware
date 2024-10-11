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

    setupDataTypes();
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
const char* insertData = "INSERT INTO Data (DataID, Timestamp, DataTypeID, Value) "
                         "VALUES (?, ?, ?, ?)";


void DataStorage::storeData(float data, DataTypes dataType){
    int exit = 0;

    sqlite3_stmt *statement;

    exit = sqlite3_prepare_v2(db, insertData, -1, &statement, nullptr);

    if(exit){
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values to parameters
    sqlite3_bind_double(statement, 1, currentSessionID);
    sqlite3_bind_double(statement, 2, currentTimestamp);
    sqlite3_bind_double(statement, 3, dataType);
    sqlite3_bind_double(statement, 4, data);


    exit = sqlite3_step(statement);

    if(exit != SQLITE_DONE){
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

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

    // We have to enable the foreign key constraint. This ensures we are setting 
    // all of the tables up correctly and will have the appropriate data.
    const char* enableForeignKeys = "PRAGMA foreign_keys = ON;";

    exit = sqlite3_exec(db, enableForeignKeys, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Enabling Foreign Keys: " << messageError << std::endl;
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
                      "DataID INTEGER,"
                      "Timestamp REAL,"
                      "DataTypeID INTEGER,"
                      "Value REAL,"
                      "FOREIGN KEY (DataID) REFERENCES Session(SessionID) ON DELETE CASCADE,"
                      "FOREIGN KEY (DataTypeID) REFERENCES DataType(DataTypeID) ON DELETE CASCADE)";

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

    dataTypesInDB.push_back(DataTypes::CAR_SPEED);
    dataTypeName.push_back("CAR SPEED");
    dataTypeUnit.push_back("m/s");


    const char* insertDataType = "INSERT OR REPLACE INTO DataType (DataTypeID, DataTypeName, DataUnit) VALUES (?, ?, ?)";

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
    }
}


    // enum DataRetrieval{
    //     RETRIEVE_ALL,
    //     RETRIEVE
    // }