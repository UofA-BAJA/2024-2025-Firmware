/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include "procedures/Procedure.h"

class Car{
    public:
        Car();
        ~Car();
        
    private:
        void init();
        void execute();
        void end();
};

#endif