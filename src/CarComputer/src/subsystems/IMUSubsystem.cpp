// #include ""

#include "IMUSubsystem.h"

IMUSubsystem::IMUSubsystem(CANDispatcher* canDispatcher){
    imu = new IMU(canDispatcher);
}


double IMUSubsystem::getRotationX(){
    imu->requestRotationX();
    return 0;
}