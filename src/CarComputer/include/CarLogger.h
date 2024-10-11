#ifndef CARLOGGER_H
#define CARLOGGER_H

#include <iostream>
#include <chrono>
#include <ctime>

class CarLogger{

    public:

        // This should only be called by the car.
        CarLogger();

        void Log(const char* str);
        void LogWarning(const char* str);
        void LogError(const char* str);
        void LogPanic(const char* str);

    private:
        const char* logPath = "/home/bajaelectrical/car.log";

};


#endif