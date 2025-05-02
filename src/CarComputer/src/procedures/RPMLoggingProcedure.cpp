#include "Procedure.h"
#include "DrivetrainSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "Coms.h"

namespace BajaWildcatRacing{

    class RPMLoggingProcedure : public Procedure {
        public:
            DrivetrainSubsystem& drivetrainSubsystem;
            DataStorage& dataStorage;
            Coms& coms;

            RPMLoggingProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
            : drivetrainSubsystem(drivetrainSubsystem)
            , dataStorage(dataStorage)
            , coms(coms){
                this->frequency = 30;
            }

            void init() override {
                std::cout << "RPM Logging procedure initialized!" << std::endl;
            }

            void execute() override {
                float engineRPM = drivetrainSubsystem.getEngineRPM();

                dataStorage.storeData(engineRPM, DataTypes::MOTOR_RPM);
                coms.sendData(DataTypes::MOTOR_RPM, engineRPM);
            }

            void end() override {
                std::cout << "RPM Logging Procedure is done for! (ended)" << std::endl;
            }

            bool isFinished() override {
                return false;
            }
    
            std::string toString() override {
                return "RPM Logging Procedure";
            }
    }
}