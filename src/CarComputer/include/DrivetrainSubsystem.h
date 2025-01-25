#ifndef DRIVETRAINSUBSYSTEM_H
#define DRIVETRAINSUBSYSTEM_H

#include "CANDispatcher.h"
#include "CarLogger.h"


#include "Temperature.h"
#include "Tachometer.h"

class DrivetrainSubsystem{
    public:

        DrivetrainSubsystem(CANDispatcher* canDispatcher);

        float getCVTTemperature();
        bool isCVTHot();

        float getEngineRPM();

    private:
        Tachometer* tachometer;

        Temperature* cvtTemperature;
        bool cvtIsHot = false;

};

#endif