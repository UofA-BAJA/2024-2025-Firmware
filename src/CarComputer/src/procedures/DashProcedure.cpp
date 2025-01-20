#include "Procedure.h"
#include "CarLogger.h"
#include "TemperatureSubsystem.h"
#include "DashSubsystem.h"
#include "CarTime.h"



class DashProcedure : public Procedure{
    public:

        DashSubsystem* dashSubsystem;
        TemperatureSubsystem* temperatureSubsystem;

        DashProcedure(DashSubsystem* dashSubsystem, TemperatureSubsystem* temperatureSubsystem){
            this->dashSubsystem = dashSubsystem;
            this->temperatureSubsystem = temperatureSubsystem;
            this->frequency = 10;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
            // dashSubsystem->sendRPM(testSpeed);
        }

        void execute() override {
            float breh = temperatureSubsystem->getTemperature();
            // std::cout << "CVT Temperature from dash: " << breh << std::endl;
            dashSubsystem->sendCVTTemp(breh);
            dashSubsystem->sendTimeSeconds(CarTime::getCurrentTimeSeconds());
        }

        void end() override {
            // testRPM = 0.0f;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Dash Procedure";
        }
    private:
};