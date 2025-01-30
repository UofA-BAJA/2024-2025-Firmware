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
            // static float time;
            Car();
            ~Car();

        private:
            void init();
            void execute();
            void end();

            const char* canInterface = "can0";
            const char* dataStoragePath = "/home/bajaelectrical/DataStorage";
            const char* logPath = "/home/bajaelectrical/car.log";

            CarContainer carContainer;
            ProcedureScheduler procedureScheduler;
            CANDispatcher canDispatcher;
            DataStorage dataStorage;
            Coms coms;

    };

}


#endif