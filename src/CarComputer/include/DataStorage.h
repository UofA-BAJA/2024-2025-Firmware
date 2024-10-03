/* 
 *
 *
 * Author: Matthew Larson
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <sqlite3.h>
#include <iostream>

#include "Subsystem.h"

class DataStorage {
    public:
        DataStorage(const char* path);
        int getData();
        void storeData(int data);
    private:
        void setupDatabase(const char* path);
};


#endif