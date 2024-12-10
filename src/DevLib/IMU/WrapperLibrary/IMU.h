#ifndef IMU_H
#define IMU_H


#include "CANDispatcher.h"
#include "functional"
#include "linux/can.h"

#include <chrono>

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
        double minimumRepeatThreshold;

        float rotX = 0;
        float rotY = 0;
        float rotZ = 0;

        float accX = 0;
        float accY = 0;
        float accZ = 0;

        std::chrono::steady_clock::time_point lastRotXTime;
        std::chrono::steady_clock::time_point lastRotYTime;
        std::chrono::steady_clock::time_point lastRotZTime;

        std::chrono::steady_clock::time_point lastAccXTime;
        std::chrono::steady_clock::time_point lastAccYTime;
        std::chrono::steady_clock::time_point lastAccZTime;

        void populateRotationX(struct can_frame frame);
        void populateRotationY(struct can_frame frame);
        void populateRotationZ(struct can_frame frame);

        void populateAccelerationX(struct can_frame frame);
        void populateAccelerationY(struct can_frame frame);
        void populateAccelerationZ(struct can_frame frame);



};


#endif