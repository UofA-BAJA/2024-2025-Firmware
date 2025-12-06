#include "Spedometer.h"

namespace BajaWildcatRacing
{
    Spedometer::Spedometer(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId){

    }

    float Spedometer::getFrontLeftRPM(){
        sendCanRequest(0x02, &frontLeftRPM, sizeof(float));
        return frontLeftRPM;
    }

    float Spedometer::getFrontRightRPM(){
        sendCanRequest(0x03, &frontRightRPM, sizeof(float));
        return frontRightRPM;
    }

    float Spedometer::getRearRPM(){
        sendCanRequest(0x01, &rearRPM, sizeof(float));
        return rearRPM;
    }

}