#ifndef TEMPERATURE_H
#define TEMPERATURE_H


#include "CANDevice.h"
#include "functional"
#include "linux/can.h"

namespace BajaWildcatRacing
{

    class Temperature : CANDevice{

        public:
            Temperature(CANDispatcher& canDispatcher);

            float getLatestTemperature();

        private:

            float temperature = 0.0;
        
    };

}

#endif