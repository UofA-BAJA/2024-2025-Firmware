#include "Temperature.h"

namespace BajaWildcatRacing
{

    float Temperature::getLatestTemperature(){

        sendCanCommand(Device::CVT_TEMP, 0x01, &temperature);
        return temperature;

    }

}
