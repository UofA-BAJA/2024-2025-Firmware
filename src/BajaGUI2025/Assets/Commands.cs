/*
 * Here are the possible commands that will be sent from the pit computer
 * to the car computer.
*/

/**
 * NOTE: IT IS EXTREMELY IMPORTANT THAT THE ENUM HERE MATCHES THE ENUM FOUND IN THE 
 * INCLUDE FOLDER OF THE BAJA ELECTRICAL 2024-2025 REPOSITORY!
*/
public enum Command{
    DEFAULT_CAR_START = 0,
    START_LOG = 100,              // Starts recording data and realtime transmission
    END_LOG = 101,                // Stops recording data and realtime transmission
    RETRIEVE = 102,               // Sends the logs to the Pit Computer
    // We don't talk about 103
    EMERGENCY_STOP = 104,         // Not sure if we'll ever use this one


    START_CAM = 105,
    ACCELERATION = 106,

    ENABLE_CHECK_ENGINE = 69,
    DISABLE_CHECK_ENGINE = 70
};
