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
            this->frequency = 3;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
            // dashSubsystem->sendRPM(testSpeed);
        }

        void execute() override {
            float breh = temperatureSubsystem->getTemperature();
            dashSubsystem->sendCVTTemp(breh);
            if(temperatureSubsystem->isHot()){
                dashSubsystem->setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, true);
            }else{
                dashSubsystem->setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, false);
            }
            dashSubsystem->sendTimeSeconds(CarTime::getCurrentTimeSeconds());
            dashSubsystem->sendIndicatorLightState();
        }

        void end() override {
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Dash Procedure";
        }
    private:
};