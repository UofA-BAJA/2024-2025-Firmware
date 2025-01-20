#ifndef TEMPERATURESUBSYSTEM_H
#define TEMPERATURESUBSYSTEM_H

#include "Temperature.h"
#include "CANDispatcher.h"

class TemperatureSubsystem{
    public:
        TemperatureSubsystem(CANDispatcher* canDispatcher);
        float getTemperature();

    private:
        Temperature* temperature;
};

#endif