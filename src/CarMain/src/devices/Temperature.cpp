#include "Temperature.h"

namespace BajaWildcatRacing
{

    Temperature::Temperature(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId){

    }

    float Temperature::getLatestTemperature(){

        sendCanRequest(0x01, &temperature, sizeof(float));
        return temperature;

    }

}
