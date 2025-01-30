/*
 *
 *  Class IMU
 *
 *         Author:  Matthew Larson & Karsten Yin
 *
 *        Purpose:  Provide an implementation of the IMU device, utilizing the
 *        CanDispatcher class to send and receive CAN commands
 *
 *  Inherits From:  None
 *
 *     Interfaces:  None
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:  
 *
 *+-----------------------------------------------------------------------
 *
 *   Constructors:  [List the names and arguments of all defined
 *                   constructors.]
 *
 *  Class Methods:  [List the names, arguments, and return types of all
 *                   public class methods.]
 *
 * Inst. Methods:  [List the names, arguments, and return types of all
 *                   public instance methods.]
 *
*/

#include "IMU.h"

namespace BajaWildcatRacing
{

    IMU::IMU(CANDispatcher* canDispatcher) : CANDevice(canDispatcher){

    }

    float IMU::getLatestAccelerationX(){

        sendCanCommand(Device::IMU, 0x04, &accX);
        return accX;

    }

    float IMU::getLatestAccelerationY(){

        sendCanCommand(Device::IMU, 0x05, &accY);
        return accY;

    }

    float IMU::getLatestAccelerationZ(){

        sendCanCommand(Device::IMU, 0x06, &accZ);
        return accZ;

    }

    float IMU::getLatestRotationX(){
        sendCanCommand(Device::IMU, 0x01, &rotX);
        return rotX;
    }

    float IMU::getLatestRotationY(){
        sendCanCommand(Device::IMU, 0x02, &rotY);
        return rotY;
    }

    float IMU::getLatestRotationZ(){
        sendCanCommand(Device::IMU, 0x03, &rotZ);
        return rotZ;
    }

}
