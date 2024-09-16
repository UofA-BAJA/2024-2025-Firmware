#ifndef IMU_H
#define IMU_H


#include "CANDispatcher.h"

class IMU{

    public:
        IMU(CANDispatcher* canDispatcher);

        double requestRotationX();
        double requestRotationY();
        double requestRotationZ();

        double requestAccelerationX();
        double requestAccelerationY();
        double requestAccelerationZ();
    private:
        CANDispatcher* canDispatcher;

};


#endif