#ifndef IMUSUBSYSTEM_H
#define IMUSUBSYSTEM_H

#include "IMU.h"
#include "CANDispatcher.h"

class IMUSubsystem{
    public:
        IMUSubsystem(CANDispatcher* canDispatcher);
        double getRotationX();
    private:
        IMU* imu;
};



#endif