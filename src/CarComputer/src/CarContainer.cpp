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

namespace BajaWildcatRacing
{

    CarContainer::CarContainer(
                               ProcedureScheduler& procedureScheduler, 
                               CANDispatcher& canDispatcher,
                               DataStorage& dataStorage,
                               Coms& coms)
    : imuSubsystem(canDispatcher)
    , drivetrainSubsystem(canDispatcher)
    , dashSubsystem(canDispatcher)
    {

         procedureScheduler.bindCommand<IMUProcedure>(
             std::unordered_set<Command>({Command::DEFAULT_CAR_START}), 
             std::unordered_set<Command>({}), 
             imuSubsystem, dataStorage, coms
        );

        procedureScheduler.bindCommand<AccelerationProcedure>(
            std::unordered_set<Command>({Command::ACCELERATION}), 
            std::unordered_set<Command>({Command::END_LOG, Command::EMERGENCY_STOP})
       );

        // procedureScheduler.bindCommand<TemperatureProcedure>(
        //      std::unordered_set<Command>({Command::DEFAULT_CAR_START}),
        //      std::unordered_set<Command>({Command::END_LOG}),
        //      drivetrainSubsystem, dataStorage, coms
        //  );

        procedureScheduler.bindCommand<DashProcedure>(
           std::unordered_set<Command>({Command::DEFAULT_CAR_START}),
           std::unordered_set<Command>({}),
           dashSubsystem, drivetrainSubsystem, imuSubsystem
        );

        procedureScheduler.bindCommand<CheckEngineProcedure>(
            std::unordered_set<Command>({Command::ENABLE_CHECK_ENGINE}),
            std::unordered_set<Command>({Command::DISABLE_CHECK_ENGINE}),
            dashSubsystem
         );

        procedureScheduler.bindCommand<SpedometerProcedure>(
            std::unordered_set<Command>({Command::DEFAULT_CAR_START}),
            std::unordered_set<Command>({}),
            drivetrainSubsystem, dataStorage, coms
        );



        CarLogger::Log("Car Started");
        std::cout << "Car Container Constructor called" << std::endl;
    }

}
