#include "TemperatureSubsystem.h"

TemperatureSubsystem::TemperatureSubsystem(CANDispatcher* canDispatcher){
    temperature = new Temperature(canDispatcher);
}

double TemperatureSubsystem::getTemperature(){
    return temperature->getLatestTemperature();
}