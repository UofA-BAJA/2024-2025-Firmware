#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

    DrivetrainSubsystem::DrivetrainSubsystem(CANDispatcher& canDispatcher)
    : cvtTemperature(canDispatcher, Device::Devices::CVT_TEMP)
    , tachometer(canDispatcher, Device::Devices::TACHOMETER)
    , spedometer(canDispatcher, Device::Devices::SPEDOMETER)
    {

    }

    float DrivetrainSubsystem::getCVTTemperature(){
        float temperature = cvtTemperature.getLatestTemperature();
        float tempFarenheit = (temperature * (9.0f / 5.0f)) + 32.0f;
        if(tempFarenheit > 200.0f) cvtIsHot = true;
        else cvtIsHot = false;
        return tempFarenheit;
    }

    bool DrivetrainSubsystem::isCVTHot(){
        return cvtIsHot;
    }

    float DrivetrainSubsystem::getEngineRPM(){
        return tachometer.getEngineRPM();
    }

    float DrivetrainSubsystem::getFrontRightRPM(){
        float rpm = spedometer.getFrontRightRPM();
        return (rpm < 0.0) ? 0.0f : rpm;
    }

    float DrivetrainSubsystem::getFrontLeftRPM(){
        float rpm = spedometer.getFrontLeftRPM();
        return (rpm < 0.0) ? 0.0f : rpm;
    }

    float DrivetrainSubsystem::getRearRPM(){
        float rpm = spedometer.getRearRPM();
        return (rpm < 0.0) ? 0.0f : rpm;
    }

    float DrivetrainSubsystem::getCarSpeedMetersSec(){
        // return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM()) / 2.0) * 0.0289f; //Magical number
        return spedometer.getFrontRightRPM() * 0.0289f;

    }

    float DrivetrainSubsystem::getCarSpeedMPH(){
        return ((spedometer.getFrontRightRPM() + spedometer.getFrontLeftRPM()) / 2.0) * 0.0647f; //Slightly different magic number
        return spedometer.getFrontRightRPM() * 0.0647f;

    }

}