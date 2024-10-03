#ifndef CARCONTAINER_H
#define CARCONTAINER_H

#include "ProcedureScheduler.h"
#include "CANDispatcher.h"
#include "DataStorage.h"

// class Car;

class CarContainer{
    public:
        CarContainer(ProcedureScheduler* procedureScheduler, CANDispatcher* canDispatcher, DataStorage* dataStorage);
};

#endif