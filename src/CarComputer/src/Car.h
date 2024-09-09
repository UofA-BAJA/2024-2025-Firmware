/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include "commands/Command.h"

class Car{
    public:
        Car();
        ~Car();

        int bindCommand(Command* command);

    private:
        void init();
        void execute();
        void end();

        static const int MAX_COMMANDS = 256;
        int numCommands = 0;
        Command *commands[MAX_COMMANDS];   // No way we have more than 256 commands.
};

#endif