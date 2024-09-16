/*
 *
 *
 * Author: Matthew Larson
*/

#ifndef CARCONTAINER_H
#define CARCONTAINER_H

#include "ProcedureScheduler.h"
#include "CANDispatcher.h"

// class Car;

class CarContainer{
    public:
        CarContainer(ProcedureScheduler& procedureScheduler, CANDispatcher* canDispatcher);
};

#endif