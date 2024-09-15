


// The hex code associated with the data type is going to be the CAN id that represents the data type.
enum DataTypes{
<<<<<<< HEAD
    RPM_FRONT_L,
    RPM_FRONT_R,
    RPM_BACK_L,
    RPM_BACK_R,
    MOTOR_RPM,
    PITCH,
    ROLL,
    BRAKE_PRESSURE_REAR,
    BRAKE_PRESSURE_FRONT,
    CAR_SPEED
};
=======
    RPM_FRONT_L = 0x100,
    RPM_FRONT_R = 0x101,
    RPM_BACK_L = 0x102,
    RPM_BACK_R = 0x103,
    MOTOR_RPM = 0x104,
    IMU_ROTATION_X = 0x105,
    IMU_ROTATION_Y = 0x106,
    IMU_ROTATION_Z = 0x107,
    BRAKE_PRESSURE_REAR = 0x109,
    BRAKE_PRESSURE_FRONT = 0x10A,
    CAR_SPEED = 0x10B
};
>>>>>>> fcbb791 (Car side of CAN configuration)
