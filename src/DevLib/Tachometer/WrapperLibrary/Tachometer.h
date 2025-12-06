#ifndef TACHOMETER_H
#define TACHOMETER_H

#include "CANDevice.h"

namespace BajaWildcatRacing
{

    class Tachometer : CANDevice {

        public:

            Tachometer(CANDispatcher& candispatcher);

            float getEngineRPM();


        private:
            float engineRPM = 0.0;

    };

}

#endif