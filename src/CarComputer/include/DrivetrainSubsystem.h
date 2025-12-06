#ifndef DRIVETRAINSUBSYSTEM_H
#define DRIVETRAINSUBSYSTEM_H

#include "CANDispatcher.h"
#include "CarLogger.h"
#include "DataTypes.h"

#include "Temperature.h"
#include "Tachometer.h"
#include "Spedometer.h"

namespace BajaWildcatRacing
{
    
    class DrivetrainSubsystem{
        public:

            DrivetrainSubsystem(CANDispatcher& canDispatcher);

            float getCVTTemperature();
            bool isCVTHot();

            float getEngineRPM();

            float getFrontRightRPM();
            float getFrontLeftRPM();
            float getRearRPM();
            float getCarSpeedMetersSec();
            float getCarSpeedMPH();

        private:
            Tachometer tachometer;

            Temperature cvtTemperature;

            Spedometer spedometer;
            
            bool cvtIsHot = false;

    };

}


#endif