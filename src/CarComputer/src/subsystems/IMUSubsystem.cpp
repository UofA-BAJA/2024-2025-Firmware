#include "IMUSubsystem.h"

IMUSubsystem::IMUSubsystem(CANDispatcher* canDispatcher){
    imu = new IMU(canDispatcher);
}


double IMUSubsystem::getRotationX(){
    return imu->getLatestRotationX();
}

double IMUSubsystem::getRotationY(){
    return imu->getLatestRotationY();
}

double IMUSubsystem::getRotationZ(){
    return imu->getLatestRotationZ();
}