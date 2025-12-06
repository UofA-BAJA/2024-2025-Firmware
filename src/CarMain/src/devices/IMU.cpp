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

    IMU::IMU(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId){

    }

    float IMU::getLatestAccelerationX(){

        sendCanRequest(0x04, &accX, sizeof(float));
        return accX;

    }

    float IMU::getLatestAccelerationY(){

        sendCanRequest(0x05, &accY, sizeof(float));
        return accY;

    }

    float IMU::getLatestAccelerationZ(){

        sendCanRequest(0x06, &accZ, sizeof(float));
        return accZ;

    }

    float IMU::getLatestRotationX(){
        sendCanRequest(0x01, &rotX, sizeof(float));
        return rotX;
    }

    float IMU::getLatestRotationY(){
        sendCanRequest(0x02, &rotY, sizeof(float));
        return rotY;
    }

    float IMU::getLatestRotationZ(){
        sendCanRequest(0x03, &rotZ, sizeof(float));
        return rotZ;
    }

}
