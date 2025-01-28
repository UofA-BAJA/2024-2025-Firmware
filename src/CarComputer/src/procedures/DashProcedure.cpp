#include "Procedure.h"
#include "CarLogger.h"
#include "DrivetrainSubsystem.h"
#include "DashSubsystem.h"
#include "CarTime.h"

namespace BajawildcatRacing
{

class DashProcedure : public Procedure{
    public:

        DashSubsystem* dashSubsystem;
        DrivetrainSubsystem* drivetrainSubsystem;
        

        DashProcedure(DashSubsystem* dashSubsystem, DrivetrainSubsystem* drivetrainSubsystem){
            this->dashSubsystem = dashSubsystem;
            this->drivetrainSubsystem = drivetrainSubsystem;
            this->frequency = 1;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
            // dashSubsystem->sendRPM(testSpeed);
        }

        void execute() override {
            dashSubsystem->sendCVTTemp(drivetrainSubsystem->getCVTTemperature());
            if(drivetrainSubsystem->isCVTHot()){
                dashSubsystem->setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, true);
            }else{
                dashSubsystem->setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, false);
            }
            dashSubsystem->sendTimeSeconds(CarTime::getCurrentTimeSeconds());
            dashSubsystem->sendIndicatorLightState();

            // dashSubsystem->sendRPM(drivetrainSubsystem->getEngineRPM());
            dashSubsystem->sendRPM(rand() % 4000);
            dashSubsystem->sendSpeed(rand() % 40);
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

}
