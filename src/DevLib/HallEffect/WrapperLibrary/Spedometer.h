#ifndef SPEDOMETER_H
#define SPEDOMETER_H

#include "CANDevice.h"
#include "Devices.h"

namespace BajaWildcatRacing
{
    class Spedometer : CANDevice {
        public: 
            Spedometer(CANDispatcher& canDispatcher, Device::Devices deviceId);
            float getRPM();

        private:
            Device::Devices deviceId;
            float RPM = 0.0;
    };
}

#endif