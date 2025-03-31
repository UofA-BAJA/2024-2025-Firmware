#include "Procedure.h"
#include "CarLogger.h"
#include "DrivetrainSubsystem.h"
#include "DashSubsystem.h"
#include "IMUSubsystem.h"
#include "CarTime.h"

namespace BajaWildcatRacing
{

    class DashProcedure : public Procedure{

        public:

            DashSubsystem& dashSubsystem;
            DrivetrainSubsystem& drivetrainSubsystem;
            IMUSubsystem& imuSubsystem;
            

            DashProcedure(DashSubsystem& dashSubsystem, DrivetrainSubsystem& drivetrainSubsystem, IMUSubsystem& imuSubsystem)
            : dashSubsystem(dashSubsystem)
            , drivetrainSubsystem(drivetrainSubsystem)
            , imuSubsystem(imuSubsystem)
            {
                this->frequency = 60;
            }

            void init() override{
                std::cout << "Dash Procedure Initialized" << std::endl;
                // dashSubsystem->sendRPM(testSpeed);
            }

            void execute() override {
                dashSubsystem.sendCVTTemp(drivetrainSubsystem.getCVTTemperature());
                if(drivetrainSubsystem.isCVTHot()){
                    dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, true);
                }else{
                    dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, false);
                }
                dashSubsystem.sendTimeSeconds(CarTime::getCurrentTimeSeconds());
                dashSubsystem.sendIndicatorLightState();

                dashSubsystem.sendSpeed(imuSubsystem.getRotationY());

                // dashSubsystem.sendRPM(drivetrainSubsystem->getEngineRPM());
                // dashSubsystem.sendRPM(rand() % 4000);
                // dashSubsystem.sendSpeed(rand() % 40);
                // dashSubsystem.sendSpeed(drivetrainSubsystem.getEngineRPM());
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
