/* 
 *
 *
 * Author: Matthew Larson
 */

#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>

class Subsystem{
    public:
        virtual std::string toString();
        // All of these functions can be overridden.
        virtual void init();
        virtual void execute();
        virtual void end();
};

#endif