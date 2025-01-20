#include "TemperatureSubsystem.h"

TemperatureSubsystem::TemperatureSubsystem(CANDispatcher* canDispatcher){
    temperature = new Temperature(canDispatcher);
}

float TemperatureSubsystem::getTemperature(){
    return temperature->getLatestTemperature();
}