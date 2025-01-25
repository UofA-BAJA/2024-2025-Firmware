#ifndef DASH_H
#define DASH_H

#include "CANDevice.h"
#include "Devices.h"

class Dash : CANDevice
{
    public:
        enum IndicatorLights{
            CAN_ERR = 0, //this is really only here to reserve it, the Pi shouldn't be lighting this one up
            LIGHT_1,
            LIGHT_2,
            LIGHT_3,
            CVT_HOT = 4,
            LIGHT_5,
            LIGHT_6,
            LIGHT_7,
            LIGHT_8, //Anything higher than this one is not currently planned to be wired up, but is there for testing
            LIGHT_9,
            LIGHT_10,
            LIGHT_11,
            LIGHT_12,
            LIGHT_13,
            LIGHT_14,
            LIGHT_15
        };
        Dash(CANDispatcher *canDispatcher);
        void sendSpeed(float speed);
        void sendRPM(float rpm);
        void sendCVTTemp(float cvtTemp);
        void sendTimeSeconds(float seconds);
        void setIndicatorLights(uint16_t lightState);
        void setSpecificIndicatorLight(IndicatorLights light, bool state);
        void sendIndicatorLightState();
        

    private:
        uint16_t indicatorLightState;

};

#endif