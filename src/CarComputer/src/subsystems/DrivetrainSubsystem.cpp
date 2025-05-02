#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    DrivetrainSubsystem::DrivetrainSubsystem(CANDispatcher& canDispatcher)
    : cvtTemperature(canDispatcher)
    , tachometer(canDispatcher)
    , spedometer(canDispatcher)
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
        return spedometer.getFrontRightRPM();
    }

    float DrivetrainSubsystem::getFrontLeftRPM(){
        return spedometer.getFrontLeftRPM();
    }

    float DrivetrainSubsystem::getRearRPM(){
        return spedometer.getRearRPM();
    }

    float DrivetrainSubsystem::getCarSpeedMetersSec(){
        return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM()) / 2.0) * 0.0289f; //Magical number

    }

    float DrivetrainSubsystem::getCarSpeedMPH(){
        return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM()) / 2.0) * 0.0647f; //Slightly different magic number
    }

}