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
                this->frequency = 5;

            }

            void init() override{
                std::cout << "Dash Procedure Initialized" << std::endl;
            }

            void execute() override {
                // Commented out lines to avoid millions of dropped commands during testing
                dashSubsystem.sendCVTTemp(drivetrainSubsystem.getCVTTemperature());
                if(drivetrainSubsystem.isCVTHot()){
                    dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, true);
                }else{
                    dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CVT_HOT, false);
                }

                dashSubsystem.sendTimeSeconds(CarTime::getCurrentTimeSeconds());
                dashSubsystem.sendIndicatorLightState();

                dashSubsystem.sendRPM(drivetrainSubsystem.getEngineRPM());
                // dashSubsystem.sendSpeed(drivetrainSubsystem.getCarSpeedMPH());

                float frontRightMPH = drivetrainSubsystem.getFrontRightRPM() * 0.0647f;
                float frontLeftMPH = drivetrainSubsystem.getFrontLeftRPM() * 0.0647f;
                dashSubsystem.sendSpeed((frontLeftMPH+frontRightMPH)/2);
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
