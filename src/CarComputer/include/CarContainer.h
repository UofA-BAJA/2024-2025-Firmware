#ifndef CARCONTAINER_H
#define CARCONTAINER_H

#include "ProcedureScheduler.h"
#include "CANDispatcher.h"
#include "DataStorage.h"
#include "Coms.h"
#include "Commands.h"
#include "CarLogger.h"

#include "procedures/IMUProcedure.cpp"
#include "procedures/DashProcedure.cpp"
#include "procedures/TemperatureProcedure.cpp"

#include "DashSubsystem.h"
#include "IMUSubsystem.h"
#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    class CarContainer{
        
        public:
            CarContainer(
                         ProcedureScheduler* procedureScheduler,
                         CANDispatcher* canDispatcher,
                         DataStorage* dataStorage,
                         Coms* coms
                        );
        private:
            IMUSubsystem* imuSubsystem;
            IMUProcedure* imuProcedure;

            DashSubsystem* dashSubsystem;
            DashProcedure* dashProcedure;

            DrivetrainSubsystem* drivetrainSubsystem;
            TemperatureProcedure* temperatureProcedure;



    };

}


#endif