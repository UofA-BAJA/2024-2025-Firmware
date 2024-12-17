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
        double minimumRepeatThreshold;

        float temperature = 0.0;
        std::chrono::steady_clock::time_point lastTemperatureTime;
       
        void populateTemperature(struct can_frame frame);
};


#endif