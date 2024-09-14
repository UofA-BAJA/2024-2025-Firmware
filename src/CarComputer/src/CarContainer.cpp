/*
 * Where all the subsystems and commands are defined and bound to the car
 *
 * Authors: Matthew Larson and
*/

#include <iostream>
#include "CarContainer.h"
#include "procedures/ExampleProcedure.cpp"
#include "Car.h"
#include "Commands.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

ExampleProcedure exampleProcedure;

CarContainer::CarContainer(ProcedureScheduler& procedureScheduler){

    procedureScheduler.bindCommand(&exampleProcedure, Command::START_LOG);

    std::cout << "Car Container Constructor called" << std::endl;

}