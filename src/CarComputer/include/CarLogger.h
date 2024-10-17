#ifndef CARLOGGER_H
#define CARLOGGER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>

class CarLogger{

    public:

        // This should only be called by the car.
        CarLogger();

        static void Log(const char* str);
        static void LogWarning(const char* str);
        static void LogError(const char* str);
        static void LogPanic(const char* str);

    private:
        const char* logPath = "/home/bajaelectrical/car.log";

};


#endif