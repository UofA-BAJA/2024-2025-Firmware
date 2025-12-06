#ifndef TACHOMETER_H
#define TACHOMETER_H

#include "CANDevice.h"

namespace BajaWildcatRacing
{

    class Tachometer : CANDevice {

        public:

            Tachometer(CANDispatcher& candispatcher, byte deviceId);

            float getEngineRPM();


        private:
            float engineRPM = 0.0;

    };

}

#endif