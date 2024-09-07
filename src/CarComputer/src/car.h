/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include "subsystems/dash.h"

class Car
{
    public:
        Car();
        void execute();
    private:
        Dash dash;
};

#endif