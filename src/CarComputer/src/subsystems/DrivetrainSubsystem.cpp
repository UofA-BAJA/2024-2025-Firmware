#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    DrivetrainSubsystem::DrivetrainSubsystem(CANDispatcher& canDispatcher)
    : cvtTemperature(canDispatcher)
    , tachometer(canDispatcher)
    {

    }

    float DrivetrainSubsystem::getCVTTemperature(){
        float temperature = cvtTemperature.getLatestTemperature();
        if(temperature > 40) cvtIsHot = true;
        else cvtIsHot = false;
        return temperature;
    }

    bool DrivetrainSubsystem::isCVTHot(){
        return cvtIsHot;
    }

    float DrivetrainSubsystem::getEngineRPM(){
        return tachometer.getEngineRPM();
    }

}