/*
 * Here are the possible commands that will be sent from the pit computer
 * to the car computer.
*/

enum Commands{
    START_LOG,              // Starts recording data and realtime transmission
    END_LOG,                // Stops recording data and realtime transmission
    RETRIEVE,               // Sends the logs to the Pit Computer
    EMERGENCY_STOP          // Not sure if we'll ever use this one
};
