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
    const char* sql = "CREATE TABLE IF NOT EXISTS SESSION("
                      "ID INT PRIMARY KEY  NOT NULL, "
                      "NAME TEXT        NOT NULL, ";

    char* messageError;
    exit = sqlite3_exec(db, sql, NULL, 0, &messageError);

    if(exit != SQLITE_OK){
        std::cerr << "Error Create Table" << std::endl;
        sqlite3_free(messageError);
    }
    else{
        std::cout << "Table created successfully" << std::endl;
    }












    sqlite3_close(db); 

    return 0;
}