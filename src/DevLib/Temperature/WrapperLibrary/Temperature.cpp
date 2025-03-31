#include "Temperature.h"

namespace BajaWildcatRacing
{

    Temperature::Temperature(CANDispatcher& canDispatcher) : CANDevice(canDispatcher){

    }

    float Temperature::getLatestTemperature(){

        sendCanCommand(Device::CVT_TEMP, 0x01, &temperature);
        return temperature;

    }

}
