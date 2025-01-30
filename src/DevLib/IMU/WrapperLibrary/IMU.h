#ifndef IMU_H
#define IMU_H


#include "CANDevice.h"
#include "functional"
// #include "linux/can.h"
// #include "Devices.h"


#include <chrono>

namespace BajaWildcatRacing
{

    class IMU : CANDevice {

        public:
            IMU(CANDispatcher* canDispatcher);

            float getLatestRotationX();
            float getLatestRotationY();
            float getLatestRotationZ();

            float getLatestAccelerationX();
            float getLatestAccelerationY();
            float getLatestAccelerationZ();
        private:

            float rotX = 0;
            float rotY = 0;
            float rotZ = 0;

            float accX = 0;
            float accY = 0;
            float accZ = 0;
    };

}



#endif