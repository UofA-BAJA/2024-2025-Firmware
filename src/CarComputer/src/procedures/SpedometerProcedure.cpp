#include "Procedure.h"

#include "DrivetrainSubsystem.h"
#include "Coms.h"
#include "DataStorage.h"
#include "CarTime.h"


namespace BajaWildcatRacing
{

class SpedometerProcedure : public Procedure {
    public:

        DrivetrainSubsystem& drivetrainSubsystem;
        DataStorage& dataStorage;
        Coms& coms;

        float deltaTime = 0;
        float prevCarTime;
        float prevCarMPS = 0;

        float distMiles = 0;

        SpedometerProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
        : drivetrainSubsystem(drivetrainSubsystem)
        , dataStorage(dataStorage)
        , coms(coms)
        {
            this->frequency = 60;
        }
        
        void init() override {

            prevCarTime = CarTime::getCurrentTimeSeconds();
            std::cout << "Spedometer procedure initialized!" << std::endl;
        }

        void execute() override {

            float mph = drivetrainSubsystem.getEngineRPM();

            dataStorage.storeData(mph, DataTypes::CAR_SPEED);

            // std::cout << "MPH: " << mph << std::endl;

            coms.sendData(DataTypes::CAR_SPEED, mph);

            float currTime = CarTime::getCurrentTimeSeconds();
            deltaTime = currTime - prevCarTime;
            prevCarTime = currTime;

            
            // Integrating using the trapezoidal rule:
            float currCarMPS = mph / 3600.0f;
            float trapezoid = (deltaTime * (currCarMPS + prevCarMPS)) * 0.5f;
            distMiles += trapezoid;

            std::cout << "Dist (miles): " << distMiles << " MPH: " << mph << std::endl;

            prevCarMPS = currCarMPS;

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

