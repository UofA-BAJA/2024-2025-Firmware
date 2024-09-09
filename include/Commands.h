/*
 * Here are the possible commands that will be sent from the pit computer
 * to the car computer.
*/

enum Commands{
    START_LOG,
    END_LOG,
    RETRIEVE,
    EMERGENCY_STOP          // Not sure if we'll ever use this one
};
