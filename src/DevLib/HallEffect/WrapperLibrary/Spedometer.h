#ifndef SPEDOMETER_H
#define SPEDOMETER_H

#include "CANDevice.h"
// #include "Devices.h"

namespace BajaWildcatRacing
{
    class Spedometer : CANDevice {
        public: 
            Spedometer(CANDispatcher& canDispatcher);
            float getFrontLeftRPM();
            float getFrontRightRPM();
            float getRearRPM();
        private:
            float frontLeftRPM = 0.0;
            float frontRightRPM = 0.0;
            float rearRPM = 0.0;
    };
}

#endif