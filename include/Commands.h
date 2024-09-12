/*
 * Here are the possible commands that will be sent from the pit computer
 * to the car computer.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

enum Command{
    START_LOG = 0,              // Starts recording data and realtime transmission
    END_LOG = 1,                // Stops recording data and realtime transmission
    RETRIEVE = 2,               // Sends the logs to the Pit Computer
    EMERGENCY_STOP = 4          // Not sure if we'll ever use this one
};


#endif