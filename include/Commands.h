/*
 * Here are the possible commands that will be sent from the pit computer
 * to the car computer.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

enum Command{
    START_LOG = 100,              // Starts recording data and realtime transmission
    END_LOG = 1,                // Stops recording data and realtime transmission
    RETRIEVE = 2,               // Sends the logs to the Pit Computer
    // We don't talk about 3
    EMERGENCY_STOP = 4,         // Not sure if we'll ever use this one

    DEFAULT_CAR_START = 0,

    START_CAM = 5
};


#endif