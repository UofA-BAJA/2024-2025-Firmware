/*
 * Where all the subsystems and commands are defined and bound to the car
 *
 * Authors: Matthew Larson and
*/

#include <iostream>
#include "CarContainer.h"
#include "Car.h"
#include "Commands.h"

#include "procedures/ExampleProcedure.cpp"
#include "procedures/TestProcedure.cpp"

#include "subsystems/DataStorage.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

ExampleProcedure* exampleProcedure;
TestProcedure* testProcedure;



CarContainer::CarContainer(ProcedureScheduler& procedureScheduler){
    procedureScheduler.bindCommand(testProcedure, Command::START_LOG);

    std::cout << "Car Container Constructor called" << std::endl;
}