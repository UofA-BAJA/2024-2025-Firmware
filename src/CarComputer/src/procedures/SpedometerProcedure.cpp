#include "Procedure.h"

#include "DrivetrainSubsystem.h"
#include "Coms.h"
#include "DataStorage.h"

namespace BajaWildcatRacing
{

class SpedometerProcedure : public Procedure {
    public:

        DrivetrainSubsystem& drivetrainSubsystem;
        DataStorage& dataStorage;
        Coms& coms;

        SpedometerProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
        : drivetrainSubsystem(drivetrainSubsystem)
        , dataStorage(dataStorage)
        , coms(coms)
        {
            this->frequency = 60;
        }
        
        void init() override {
            std::cout << "Spedometer procedure initialized!" << std::endl;
        }

        void execute() override {

            float mph = drivetrainSubsystem.getEngineRPM();

            dataStorage.storeData(mph, DataTypes::CAR_SPEED);

            // std::cout << "MPH: " << mph << std::endl;

            coms.sendData(DataTypes::CAR_SPEED, mph);

        }

        void end() override {
            std::cout << "Spedometer procedure ended" << std::endl;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Example Procedure";
        }

    private:

};

}

