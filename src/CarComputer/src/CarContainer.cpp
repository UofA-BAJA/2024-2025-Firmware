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
#include "procedures/IMUProcedure.cpp"

#include "subsystems/DataStorage.h"
#include "subsystems/IMUSubsystem.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

// ExampleProcedure* exampleProcedure;
// TestProcedure* testProcedure;

IMUSubsystem* imuSubsystem;
IMUProcedure* imuProcedure;

// DataStorage dataStorageSubsystem;


CarContainer::CarContainer(ProcedureScheduler& procedureScheduler, CANDispatcher* canDispatcher){

    // testProcedure = new TestProcedure(dataStorageSubsystem);
    imuSubsystem = new IMUSubsystem(canDispatcher);
    imuProcedure = new IMUProcedure(imuSubsystem);

    procedureScheduler.bindCommand(imuProcedure, Command::START_LOG);

    std::cout << "Car Container Constructor called" << std::endl;
}