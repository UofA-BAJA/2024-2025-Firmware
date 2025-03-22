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
#include "procedures/SpedometerProcedure.cpp"
#include "procedures/AccelerationProcedure.cpp"

#include "DashSubsystem.h"
#include "IMUSubsystem.h"
#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    class CarContainer{
        
        public:
            CarContainer(
                         ProcedureScheduler& procedureScheduler,
                         CANDispatcher& canDispatcher,
                         DataStorage& dataStorage,
                         Coms& coms
                        );
        private:
            IMUSubsystem imuSubsystem;

            DashSubsystem dashSubsystem;

            DrivetrainSubsystem drivetrainSubsystem;

    };

}


#endif