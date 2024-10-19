/* 
 * Testing out Sqlite3 functionality.
 *
 * Command to compile (on linux): g++ main.cpp -o main -l sqlite3
 *
 * Make sure you have the libsqlite3-dev package installed
 * 
 * Author: Matthew Larson
*/

#include <sqlite3.h>
#include <iostream>
// #include "DataTypes.h"

#include <sqlite3.h>
#include <iostream>
// #include "DataTypes.h"



int main(){

    const char* fileName = "idk.db";

    // Database handle for our sqlite database
    sqlite3* db;

    int exit = 0;
    // If database does not already exist, it will be created.
    exit = sqlite3_open(fileName, &db);

    if (exit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl; 
        return (-1); 
    } 
    else
        std::cout << "Opened Database Successfully!" << std::endl; 

    // The command written in SQL language
    const char* sql = "CREATE TABLE IF NOT EXISTS Session("
                      "SessionID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "StartTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                      "EndTime TIMESTAMP,"
                      "Name TEXT        NOT NULL)";

    char* messageError;
    exit = sqlite3_exec(db, sql, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Create Table: " << messageError << std::endl;
        sqlite3_free(messageError);
    }
    else{
        std::cout << "Session table created successfully" << std::endl;
    }

    // Create Timestamp Table
    sql = "CREATE TABLE IF NOT EXISTS Timestamp("
                      "TimestampID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Time REAL)";

    exit = sqlite3_exec(db, sql, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Create Table: " << messageError << std::endl;
        sqlite3_free(messageError);
    }
    else{
        std::cout << "Timestamp table created successfully" << std::endl;
    }

    // Create DataTypes Table
    sql = "CREATE TABLE IF NOT EXISTS DataType("
                      "DataTypeID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "DataTypeName TEXT,"
                      "DataUnit TEXT)";

    exit = sqlite3_exec(db, sql, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Create Table: " << messageError << std::endl;
        sqlite3_free(messageError);
    }
    else{
        std::cout << "DataType table created successfully" << std::endl;
    }


    // Create Data Table
    sql = "CREATE TABLE IF NOT EXISTS Data("
                      "DataID INTEGER,"
                      "TimestampID INTEGER,"
                      "DataTypeID INTEGER,"
                      "Value REAL,"
                      "FOREIGN KEY (DataID) REFERENCES Session(SessionID) ON DELETE CASCADE,"
                      "FOREIGN KEY (TimestampID) REFERENCES Timestamp(TimestampID) ON DELETE CASCADE,"
                      "FOREIGN KEY (DataTypeID) REFERENCES DataType(DataTypeID) ON DELETE CASCADE)";

    exit = sqlite3_exec(db, sql, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Create Table: " << messageError << std::endl;
        sqlite3_free(messageError);
    }
    else{
        std::cout << "Data table created successfully" << std::endl;
    }





    // Testing of inserting data into the database =D









    sqlite3_close(db); 

    return 0;
}