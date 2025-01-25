#include "Temperature.h"

float Temperature::getLatestTemperature(){

    sendCanCommand(Device::CVT_TEMP, 0x01, &temperature);
    return temperature;

}