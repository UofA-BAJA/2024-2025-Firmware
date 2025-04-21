#include "Spedometer.h"

namespace BajaWildcatRacing
{
    Spedometer::Spedometer(CANDispatcher& canDispatcher, Device::Devices deviceId) : CANDevice(canDispatcher){
        this->deviceId = deviceId;
    }

    float Spedometer::getRPM(){
        sendCanCommand(deviceId, 0x01, &RPM);
        return RPM;
    }

}