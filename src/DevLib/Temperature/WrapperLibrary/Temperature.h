#ifndef TEMPERATURE_H
#define TEMPERATURE_H


#include "CANDispatcher.h"
#include "functional"
#include "linux/can.h"

class Temperature{

    public:
        Temperature(CANDispatcher* canDispatcher);

        float getLatestTemperature();
    private:
        CANDispatcher* canDispatcher;

        float temperature = 0.0;

        void populateTemperature(struct can_frame frame)



};


#endif