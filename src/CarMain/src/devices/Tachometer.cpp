

#include "Tachometer.h"

namespace BajaWildcatRacing
{

    Tachometer::Tachometer(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId) {

    }

    float Tachometer::getEngineRPM(){
        sendCanRequest(0x01, &engineRPM, sizeof(float));
        return engineRPM;
    }

}
