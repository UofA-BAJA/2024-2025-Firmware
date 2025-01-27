/*
 *
 *  Class: CarContainer
 *
 *         Author:  Matthew Larson, Karsten Yin, Julian Rendon
 *
 *        Purpose:  Defines the functionality of the car. All subsystems and procedures are defined,
 *        created, and bound here. 
 *
 *  Inherits From:  None
 *
 *     Interfaces:  None
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:  None
 *
 *+-----------------------------------------------------------------------
 *
 *   Constructors:  CarContainer(ProcedureScheduler* procedureScheduler, CANDispatcher* canDispatcher, DataStorage* dataStorage, Coms* coms) --
 *   the location to define all the subsystems and procedures in the car and subsequently bind the procedures to commands.
 *
 *  Class Methods:  [List the names, arguments, and return types of all
 *                   public class methods.]
 *
 * Inst. Methods:  [List the names, arguments, and return types of all
 *                   public instance methods.]
 *
*/


#include <iostream>

#include "CarContainer.h"

CarContainer::CarContainer(ProcedureScheduler* procedureScheduler, CANDispatcher* canDispatcher, DataStorage* dataStorage, Coms* coms){

    imuSubsystem = new IMUSubsystem(canDispatcher);
    imuProcedure = new IMUProcedure(imuSubsystem, dataStorage, coms);

    drivetrainSubsystem = new DrivetrainSubsystem(canDispatcher);
    temperatureProcedure = new TemperatureProcedure(drivetrainSubsystem, dataStorage, coms);

    dashSubsystem = new DashSubsystem(canDispatcher);
    dashProcedure = new DashProcedure(dashSubsystem, drivetrainSubsystem);

    // procedureScheduler->bindCommand(imuProcedure, Command::DEFAULT_CAR_START);
    // procedureScheduler->bindCommand(temperatureProcedure, Command::DEFAULT_CAR_START);
    procedureScheduler->bindCommand(dashProcedure, Command::DEFAULT_CAR_START);


    CarLogger::Log("Car Started");
    std::cout << "Car Container Constructor called" << std::endl;
}