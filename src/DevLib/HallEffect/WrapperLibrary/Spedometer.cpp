#include "Spedometer.h"

namespace BajaWildcatRacing
{
    Spedometer::Spedometer(CANDispatcher& canDispatcher) : CANDevice(canDispatcher){

    }

    float Spedometer::getFrontLeftRPM(){
        sendCanCommand(0x05, 0x01, &frontLeftRPM);
        return frontLeftRPM;
    }

}