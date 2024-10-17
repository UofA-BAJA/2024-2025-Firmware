#include "CarLogger.h"


CarLogger::CarLogger(){
    std::ofstream logFile(logPath);

    logFile << "Testing stuff!" << std::endl;
}

void CarLogger::Log(const char* str){

}


void CarLogger::LogWarning(const char* str){
    
}

void CarLogger::LogError(const char* str){
    
}

void CarLogger::LogPanic(const char* str){
    

}