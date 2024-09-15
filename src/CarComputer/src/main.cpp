/* 
 * Not much here, just initializing the car.
 * This file is the only place where there should
 * be a main function.
 * 
 * Author: Matthew Larson
 */

#include "Car.h"
#include "CarContainer.h"

#include <libsocketcan.h>


int main(){
    Car car = Car();

    return 0;
}