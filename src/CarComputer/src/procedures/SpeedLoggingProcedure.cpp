#include "Procedure.h"
#include "DrivetrainSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "Coms.h"

namespace BajaWildcatRacing{

    class SpeedLoggingProcedure : public Procedure {
        public:
            DrivetrainSubsystem& drivetrainSubsystem;
            DataStorage& dataStorage;
            Coms& coms;

            SpeedLoggingProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
            : drivetrainSubsystem(drivetrainSubsystem)
            , dataStorage(dataStorage)
            , coms(coms){
                this->frequency = 30;
            }

            void init() override {
                std::cout << "Speed Logging procedure initialized!" << std::endl;
            }

            void execute() override {
                float fr = drivetrainSubsystem.getFrontRightRPM();
                float fl = drivetrainSubsystem.getFrontLeftRPM();
                float rear = drivetrainSubsystem.getRearRPM();

                float mps = drivetrainSubsystem.getCarSpeedMetersSec();

                dataStorage.storeData(mps, DataTypes::CAR_SPEED);
                dataStorage.storeData(fr, DataTypes::RPM_FRONT_R);
                dataStorage.storeData(fl, DataTypes::RPM_FRONT_L);
                dataStorage.storeData(rear, DataTypes::RPM_BACK);

                coms.sendData(DataTypes::CAR_SPEED, mps);
                coms.sendData(DataTypes::RPM_FRONT_R, fr);
                coms.sendData(DataTypes::RPM_FRONT_L, fl);
                coms.sendData(DataTypes::RPM_BACK, rear);
 
            }

            void end() override {
                std::cout << "Speed Logging Procedure is done for! (ended)" << std::endl;
            }

            bool isFinished() override {
                return false;
            }
    
            std::string toString() override {
                return "Speed Logging Procedure";
            }
    };
}