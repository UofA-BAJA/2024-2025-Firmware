#ifndef CARLOGGER_H
#define CARLOGGER_H

class CarLogger{

    public:

        void Log(const char* str);
        void LogWarning(const char* str);
        void LogError(const char* str);
        void LogPanic(const char* str);

    private:
        const char* logPath = "/home/bajaelectrical/car.log";

};


#endif