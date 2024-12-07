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
#include "procedures/TemperatureProcedure.cpp"

#include "DataStorage.h"
#include "IMUSubsystem.h"
#include "TemperatureSubsystem.h"

/*
 * Here you create the subsystems and commands.
 * You should also bind the commands to the car or they will not
 * do anything.
*/

// ExampleProcedure* exampleProcedure;
// TestProcedure* testProcedure;

IMUSubsystem* imuSubsystem;
IMUProcedure* imuProcedure;

TemperatureSubsystem* temperatureSubsystem;
TemperatureProcedure* temperatureProcedure;

CarContainer::CarContainer(ProcedureScheduler* procedureScheduler, CANDispatcher* canDispatcher, DataStorage* dataStorage, Coms* coms){

    // testProcedure = new TestProcedure(dataStorageSubsystem);
    imuSubsystem = new IMUSubsystem(canDispatcher);
    imuProcedure = new IMUProcedure(imuSubsystem, dataStorage, coms);

    temperatureSubsystem = new TemperatureSubsystem(canDispatcher);
    temperatureProcedure = new TemperatureProcedure(temperatureSubsystem, dataStorage, coms);

    procedureScheduler->bindCommand(imuProcedure, Command::START_LOG);
    procedureScheduler->bindCommand(temperatureProcedure, Command::START_LOG);

    std::cout << "Car Container Constructor called" << std::endl;
}