/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <time.h>
#include <cstdlib>

#include "Car.h"
#include "CarContainer.h"
#include "ProcedureScheduler.h"
#include "CANDispatcher.h"
#include "Procedure.h"
#include "DataStorage.h"

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