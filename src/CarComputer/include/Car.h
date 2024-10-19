/* 
 * Defines all of the subsystems on the car
 * 
 * 
 * Author: Matthew Larson
 */

#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <chrono>
#include <cstdlib>

#include "CarContainer.h"
#include "ProcedureScheduler.h"
#include "CANDispatcher.h"
#include "Procedure.h"
#include "DataStorage.h"
#include "CarLogger.h"

#include "Constants.h"

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