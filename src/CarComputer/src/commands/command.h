/*
 * 
 *
 * Matthew Larson
*/


#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command{
    public:
        virtual std::string toString();
        // All of these functions can be overridden.
        virtual void init();
        virtual void execute();
        virtual void end();
};

#endif