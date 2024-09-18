#include "IMUSubsystem.h"

IMUSubsystem::IMUSubsystem(CANDispatcher* canDispatcher){
    imu = new IMU(canDispatcher);
}


double IMUSubsystem::getRotationX(){
    return imu->getLatestRotationX();
}