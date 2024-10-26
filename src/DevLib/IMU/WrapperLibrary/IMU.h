#ifndef IMU_H
#define IMU_H


#include "CANDispatcher.h"
#include "functional"
#include "linux/can.h"

class IMU{

    public:
        IMU(CANDispatcher* canDispatcher);

        float getLatestRotationX();
        float getLatestRotationY();
        float getLatestRotationZ();

        float getLatestAccelerationX();
        float getLatestAccelerationY();
        float getLatestAccelerationZ();
    private:
        CANDispatcher* canDispatcher;

        float rotX = 0;
        float rotY = 0;
        float rotZ = 0;

        float accX = 0;
        float accY = 0;
        float accZ = 0;

        void populateRotationX(struct can_frame frame);
        void populateRotationY(struct can_frame frame);
        void populateRotationZ(struct can_frame frame);

        void populateAccelerationX(struct can_frame frame);
        void populateAccelerationY(struct can_frame frame);
        void populateAccelerationZ(struct can_frame frame);



};


#endif