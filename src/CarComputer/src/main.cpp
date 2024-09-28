/* 
 * Not much here, just initializing the car.
 * This file is the only place where there should
 * be a main function.
 * 
 * Author: Matthew Larson
 */

#include "Car.h"
#include "CarContainer.h"

#include <unistd.h>
#include <iostream>
#include <cstdlib>

unsigned int microseconds = 1000000;

int main(){

    std::string canDownCommand = "sudo ip link set can0 down";
    std::string canUpCommand = "sudo ip link set can0 up";

    int result1 = std::system(canDownCommand.c_str());

    if(result1 == 0){
        std::cout << "Can down executed successfully" << std::endl;
    }

    int result2 = std::system(canUpCommand.c_str());

    if(result2 == 0){
        std::cout << "Can up executed successfully" << std::endl;
    }



    // usleep(500000);

    Car car = Car();



    return 0;
}