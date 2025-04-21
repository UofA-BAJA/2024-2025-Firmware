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
        //Average of all 3 multipled by some constant to convert Rotations to Meters and 1/60 for per min to per sec 
        return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM() + spedometer.getRearRPM()) / 3.0) * (1 / 60);
    }

    float DrivetrainSubsystem::getCarSpeedMPH(){
        //Average of all 3 multipled by some constant to convert Rotations to Miles and 60 for per min to per hour 
        return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM() + spedometer.getRearRPM()) / 3.0) * 1 * 60;
    }

}