/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include "commands/command.h"


class Car
{
    public:
        Car();
        ~Car();

        int BindCommand(Command command);

    private:
        void init();
        void execute();
        void end();

        static const int MAX_COMMANDS = 256;
        int numCommands = 0;
        Command commands[MAX_COMMANDS];   // No way we have more than 32 subsystems. Even 8 would be excessive.
};

#endif