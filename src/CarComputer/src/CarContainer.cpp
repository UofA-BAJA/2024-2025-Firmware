/*
 * Where all the subsystems and commands are defined and bound to the car
 *
 * Authors: Matthew Larson and
*/

#include <iostream>
#include "CarContainer.h"
#include "commands/testCommand.cpp"
#include "Car.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

TestCommand testCommand;

CarContainer::CarContainer(Car& car){

    car.bindCommand(&testCommand);

    std::cout << "Car Container Constructor called" << std::endl;

}