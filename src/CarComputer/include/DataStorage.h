/* 
 *
 *
 * Author: Matthew Larson
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "Subsystem.h"


class DataStorage : public Subsystem{
    public:
        int getData();
        void storeData(int data);
};


#endif