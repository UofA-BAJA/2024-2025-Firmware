/* 
 *
 *
 * Author: Matthew Larson
 */

#include <iostream>
#include "DataStorage.h"


int i = 0;

int DataStorage::getData(){
    std::cout << "Get Data Called, i: " << i << std::endl;
    return i;
}

void DataStorage::storeData(int data){
    i = data;
    std::cout << "Store Data Called, i: " << i << std::endl;
}