#include "TemperatureSubsystem.h"
#include "CarLogger.h"

TemperatureSubsystem::TemperatureSubsystem(CANDispatcher* canDispatcher){
    temperatureDevice = new Temperature(canDispatcher);
}

float TemperatureSubsystem::getTemperature(){
    float temperature = temperatureDevice->getLatestTemperature();
    if(temperature > 40) isCVTHot = true;
    else isCVTHot = false;
    return temperature;
}

bool TemperatureSubsystem::isHot(){
    return isCVTHot;
}

