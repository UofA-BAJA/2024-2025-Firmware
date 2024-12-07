
#ifndef DATATYPES_H
#define DATATYPES_H

// The hex code associated with the data type

// This code will be used by both the pit side and the car side
// It is also the code that will serve as the "smift amount" for the mask value,
// as described in the live data packet protocol 
enum DataTypes{
    RPM_FRONT_L = 0x000,
    RPM_FRONT_R = 0x001,
    RPM_BACK_L = 0x002,
    RPM_BACK_R = 0x003,
    MOTOR_RPM = 0x004,
    IMU_ROTATION_X = 0x005,
    IMU_ROTATION_Y = 0x006,
    IMU_ROTATION_Z = 0x007,
    //THE_FRIENDS_WE_MADE_ALONG_THE_WAY = 0x108,
    BRAKE_PRESSURE_REAR = 0x008,
    BRAKE_PRESSURE_FRONT = 0x009,
    CAR_SPEED = 0x00A,
    IMU_ACCELERATION_X = 0x00B,
    IMU_ACCELERATION_Y = 0x00C,
    IMU_ACCELERATION_Z = 0x00D,
    CVT_TEMP = 0x00E,
    // The reserves are here because we can have a total of 32 data types that can be transmitted live
    RESERVE_16,
    RESERVE_17,
    RESERVE_18,
    RESERVE_19,
    RESERVE_20,
    RESERVE_21,
    RESERVE_22,
    RESERVE_23,
    RESERVE_24,
    RESERVE_25,
    RESERVE_26,
    RESERVE_27,
    RESERVE_28,
    RESERVE_29,
    RESERVE_30,
    RESERVE_31,
    RESERVE_32
    // Do not add more data types! Replace one of the reserved slots with it!
};

#endif