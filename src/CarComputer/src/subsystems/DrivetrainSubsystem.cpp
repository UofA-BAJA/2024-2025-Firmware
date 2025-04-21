#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    DrivetrainSubsystem::DrivetrainSubsystem(CANDispatcher& canDispatcher)
    : cvtTemperature(canDispatcher)
    , tachometer(canDispatcher)
    , frontRight(canDispatcher, Device::Devices::SPEDO_FR)
    , frontLeft(canDispatcher, Device::Devices::SPEDO_FL)
    , rear(canDispatcher, Device::Devices::SPEDO_REAR)
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

    float DrivetrainSubsystem::getFrontRightRPM(){
        return frontRight.getRPM();
    }

    float DrivetrainSubsystem::getFrontLeftRPM(){
        return frontLeft.getRPM();
    }

    float DrivetrainSubsystem::getRearRPM(){
        return rear.getRPM();
    }

}