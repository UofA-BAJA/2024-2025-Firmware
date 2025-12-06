#include "CarLogger.h"

namespace BajaWildcatRacing
{

    std::ofstream CarLogger::logFile;


    void CarLogger::Initialize(const char* path){
        // Set the file to be an output and append
        logFile.open(path, std::ios::out | std::ios::app);
    }

    void CarLogger::Log(const char* str){
        if(!logFile.is_open()) return;

        // Get the current time
        std::time_t currentTime = std::time(nullptr);

        // Convert time to C-string
        char* timeStr = std::ctime(&currentTime);

        // Remove the newline character at the end
        size_t len = std::strlen(timeStr);
        if(len > 0 && timeStr[len - 1] == '\n') {
            timeStr[len - 1] = '\0';
        }

        // Write to the log file
        logFile << "["<< timeStr << "] ";
        logFile << str << std::endl;

    }


    void CarLogger::LogWarning(const char* str){
        
        if(!logFile.is_open()) return;
        // Get the current time
        std::time_t currentTime = std::time(nullptr);

        // Convert time to C-string
        char* timeStr = std::ctime(&currentTime);

        // Remove the newline character at the end
        size_t len = std::strlen(timeStr);
        if(len > 0 && timeStr[len - 1] == '\n') {
            timeStr[len - 1] = '\0';
        }

        // Write to the log file
        logFile << "["<< timeStr << "] ";
        logFile << "Warning: ";
        logFile << str << std::endl;

    }

    void CarLogger::LogError(const char* str){
        if(!logFile.is_open()) return;

        // Get the current time
        std::time_t currentTime = std::time(nullptr);

        // Convert time to C-string
        char* timeStr = std::ctime(&currentTime);

        // Remove the newline character at the end
        size_t len = std::strlen(timeStr);
        if(len > 0 && timeStr[len - 1] == '\n') {
            timeStr[len - 1] = '\0';
        }

        // Write to the log file
        logFile << "["<< timeStr << "] ";
        logFile << "Error: ";
        logFile << str << std::endl;

    }

    void CarLogger::LogPanic(const char* str){
        if(!logFile.is_open()) return;

        // Get the current time
        std::time_t currentTime = std::time(nullptr);

        // Convert time to C-string
        char* timeStr = std::ctime(&currentTime);

        // Remove the newline character at the end
        size_t len = std::strlen(timeStr);
        if(len > 0 && timeStr[len - 1] == '\n') {
            timeStr[len - 1] = '\0';
        }

        // Write to the log file
        logFile << "["<< timeStr << "] ";
        logFile << "PANIC!: ";
        logFile << str << std::endl;
    }

}
