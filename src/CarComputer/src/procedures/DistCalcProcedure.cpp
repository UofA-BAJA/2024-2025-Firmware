#include "Procedure.h"

#include "DrivetrainSubsystem.h"
#include "Coms.h"
#include "DataStorage.h"
#include "CarTime.h"

#define PI 3.1415926

namespace BajaWildcatRacing
{

class DistCalcProcedure : public Procedure {
    public:

        DrivetrainSubsystem& drivetrainSubsystem;
        DataStorage& dataStorage;
        Coms& coms;

        float deltaTime = 0;
        float prevCarTime;
        // MPS = meters per second
        float prevCarMPS = 0;

        float distMeters = 0;

        DistCalcProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
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

            float rpm = drivetrainSubsystem.getRearRPM();

            // TODO:
            // mps - meters per second. I think this calculation is incorrect
            float mps = (rpm / (2 * PI * 60)) * WHEEL_RADIUS_METERS;


            float currTime = CarTime::getCurrentTimeSeconds();
            deltaTime = currTime - prevCarTime;
            prevCarTime = currTime;

            
            // Integrating using the trapezoidal rule:
            float trapezoid = (deltaTime * (mps + prevCarMPS)) * 0.5f;
            distMeters += trapezoid;

            // std::cout << "Dist (miles): " << distMiles << " MPH: " << mph << std::endl;

            dataStorage.storeData(distMeters, DataTypes::DISTANCE);
            coms.sendData(DataTypes::DISTANCE, distMeters);

            prevCarMPS = mps;

        }

        void end() override {
            std::cout << "Distance Calculation procedure ended" << std::endl;
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

