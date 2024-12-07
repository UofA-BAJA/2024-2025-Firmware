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
#include "procedures/IMUProcedure.cpp"
#include "procedures/DashProcedure.cpp"

#include "DataStorage.h"
#include "IMUSubsystem.h"
#include "DashSubsystem.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

// ExampleProcedure* exampleProcedure;
// TestProcedure* testProcedure;

IMUSubsystem* imuSubsystem;
IMUProcedure* imuProcedure;

DashSubsystem* dashSubsystem;
DashProcedure* dashProcedure;

CarContainer::CarContainer(ProcedureScheduler* procedureScheduler, CANDispatcher* canDispatcher, DataStorage* dataStorage){

    // testProcedure = new TestProcedure(dataStorageSubsystem);
    imuSubsystem = new IMUSubsystem(canDispatcher);
    imuProcedure = new IMUProcedure(imuSubsystem, dataStorage);

    procedureScheduler->bindCommand(imuProcedure, Command::START_LOG);

    dashS

    std::cout << "Car Container Constructor called" << std::endl;
}