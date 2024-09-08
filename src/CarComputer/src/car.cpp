/* 
 * Represents the car as a whole, controlling the various subsystems
 *
 * 
 * Author: Matthew Larson
 */

#include "car.h"
#include <iostream>

// Initialize Subsystems here
Car::Car(){
    dash = Dash();
}

void Car::execute(){
    std::cout << "Help\n";
}