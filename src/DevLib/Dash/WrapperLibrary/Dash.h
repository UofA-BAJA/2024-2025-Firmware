#ifndef DASH_H
#define DASH_H

#include "CANDevice.h"
#include "Devices.h"
 
namespace BajaWildcatRacing
{

    class Dash : CANDevice
    {
        public:
            enum IndicatorLights{
                FL_LOCK = 0, //reserved for use on ESP32
                CHECK_ENGINE = 1,
                FR_LOCK = 2,
                LIGHT_3,
                REAR_LOCK = 4,
                LIGHT_5,
                LIGHT_6,
                LIGHT_7,
                NO_CAN = 8, 
                RESET_WARNING = 9, //reserved for use on ESP32
                LIGHT_10,
                BAJA = 11,
                CVT_HOT = 12,
                WILDCAT = 13,
                LIGHT_14,
                RACING = 15
            };
            Dash(CANDispatcher& canDispatcher);
            void sendSpeed(float speed);
            void sendRPM(float rpm);
            void sendCVTTemp(float cvtTemp);
            void sendTimeSeconds(float seconds);
            void sendDistance(float distance);
            void setIndicatorLights(uint16_t lightState);
            void setSpecificIndicatorLight(IndicatorLights light, bool state);
            void sendIndicatorLightState();
            

        private:
            uint16_t indicatorLightState;

    };

}


#endif