#ifndef DASH_H
#define DASH_H

#include "CANDispatcher.h"
#include "Devices.h"


class Dash
{
    public:
        Dash(CANDispatcher *canDispatcher);

        void sendSpeed(float speed);
        void sendRPM(float rpm);
        void sendCVTTemp(float cvtTemp);
        void sendTimeSeconds(unsigned long seconds);
        void setIndicatorLights(uint16_t lightState);

        void iWantToDie(float help);

    private:
        CANDispatcher *canDispatcher;

};

#endif