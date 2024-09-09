/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include "subsystems/subsystem.h"

class Car
{
    public:
        Car();
        ~Car();

        int BindSubsystem(Subsystem subsystem);

    private:
        void init();
        void execute();
        void end();

        int numSubsystems = 0;
        Subsystem subsystems[32];   // No way we have more than 32 subsystems. Even 8 would be excessive.
};

#endif