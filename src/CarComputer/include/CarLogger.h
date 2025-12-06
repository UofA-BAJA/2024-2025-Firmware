#ifndef CARLOGGER_H
#define CARLOGGER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstring>

namespace BajaWildcatRacing
{

    class CarLogger{

        public:

            // This should only be called by the car.
            static void Initialize(const char* path);
            static void Log(const char* str);
            static void LogWarning(const char* str);
            static void LogError(const char* str);
            static void LogPanic(const char* str);

        private:
            static std::ofstream logFile;
    };

}

#endif