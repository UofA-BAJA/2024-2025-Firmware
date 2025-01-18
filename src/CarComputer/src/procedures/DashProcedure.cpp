#include "Procedure.h"
#include "CarLogger.h"
#include "TemperatureSubsystem.h"
#include "DashSubsystem.h"
#include "CarTime.h"



class DashProcedure : public Procedure{
    public:

        DashSubsystem* dashSubsystem;
        TemperatureSubsystem* temperatureSubsystem;

        DashProcedure(DashSubsystem* dashSubsystem, TemperatureSubsystem temperatureSubsystem){
            this->dashSubsystem = dashSubsystem;
            this->temperatureSubsystem = temperatureSubsystem;
            this->frequency = 20;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
            // dashSubsystem->sendRPM(testSpeed);
        }

        void execute() override {
            dashSubsystem->sendCVTTemp(temperatureSubsystem->getTemperature());
            dashSubsystem->sendTimeSeconds(CarTime::getCurrentTimeSeconds());
        }

        void end() override {
            // testSpeed = 0.0f;
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