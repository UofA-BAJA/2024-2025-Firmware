#ifndef DEVICES_H
#define DEVICES_H

namespace Device
{

enum Devices
{
    IMU = 0x001,
    CVT_TEMP = 0x002,
    DASH = 0x003,
    TACHOMETER = 0x04,
    SPEDO_FL = 0x05,
    SPEDO_FR = 0x06,
    SPEDO_REAR = 0x07
};

}


#endif