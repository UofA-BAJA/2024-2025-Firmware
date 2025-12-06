// The hex code associated with the data type

// This code will be used by both the pit side and the car side
// It is also the code that will serve as the "smift amount" for the mask value,
// as described in the live data packet protocol 

/**
 * NOTE: IT IS EXTREMELY IMPORTANT THAT THE ENUM HERE MATCHES THE ENUM FOUND IN THE 
 * INCLUDE FOLDER OF THE BAJA ELECTRICAL 2024-2025 REPOSITORY!
*/

public enum DataType{
    RPM_FRONT_L,
    RPM_FRONT_R,
    RPM_BACK,
    MOTOR_RPM,
    IMU_ROTATION_X,
    IMU_ROTATION_Y,
    IMU_ROTATION_Z,
    //THE_FRIENDS_WE_MADE_ALONG_THE_WAY = 0x108,
    BRAKE_PRESSURE_REAR,
    BRAKE_PRESSURE_FRONT,
    CAR_SPEED,
    IMU_ACCELERATION_X,
    IMU_ACCELERATION_Y,
    IMU_ACCELERATION_Z,
    CVT_TEMPERATURE,
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
