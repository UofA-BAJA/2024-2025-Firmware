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
#include "Coms.h"

#include "Constants.h"

#include "CarTime.h"

namespace BajaWildcatRacing
{

    class Car{

        public:
            Car();
            ~Car();

        private:
            void init();
            void execute();
            void end();

            const char* canInterface = "can0";
            const char* dataStoragePath = "/home/bajaelectrical/DataStorage";
            const char* logPath = "/home/bajaelectrical/car.log";

            // IMPORTANT NOTE!!!
            // c++ initializes members in the order they're declared in the class, not in the order they are listed
            // in the initialization list! It's very important that the order stays this way, or maybe I should make
            // it so that the constructors aren't dependent on any of the objects here
            DataStorage dataStorage;
            ProcedureScheduler procedureScheduler;
            CANDispatcher canDispatcher;            
            Coms coms;                              // coms needs the procedure scheduler
            CarContainer carContainer;              // carContainer needs all four of the above

    };

}


#endif