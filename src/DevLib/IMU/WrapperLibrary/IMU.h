#ifndef IMU_H
#define IMU_H


#include "CANDispatcher.h"
#include "functional"
#include "linux/can.h"

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

        void getRotationX(struct can_frame frame);


};


#endif