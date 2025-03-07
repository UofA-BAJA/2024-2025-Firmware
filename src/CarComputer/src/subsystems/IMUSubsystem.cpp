#include "IMUSubsystem.h"


namespace BajaWildcatRacing
{

    IMUSubsystem::IMUSubsystem(CANDispatcher& canDispatcher) : imu(canDispatcher) {


    }

    /*
    *  Method:  getRotationX
    *
    *  Purpose: provides the IMU x value in regards to ration
    *
    *  Pre-Condition:  None
    * 
    *  Post-Condition: None
    * 
    *  Parameters:  None
    *
    *  Returns: A double of the x value of the rotaiton
    *
    */
    double IMUSubsystem::getRotationX(){
        return imu.getLatestRotationX();
    }

    double IMUSubsystem::getRotationY(){
        return imu.getLatestRotationY();
    }

    double IMUSubsystem::getRotationZ(){
        return imu.getLatestRotationZ();
    }

    double IMUSubsystem::getAccelerationX(){
        return imu.getLatestAccelerationX();
    }

    double IMUSubsystem::getAccelerationY(){
        return imu.getLatestAccelerationY();
    }

    double IMUSubsystem::getAccelerationZ(){
        return imu.getLatestAccelerationZ();
    }

}
