#include "Procedure.h"

#include "DrivetrainSubsystem.h"
#include "DashSubsystem.h"
#include "Coms.h"
#include "DataStorage.h"
#include "CarTime.h"

#define PI 3.1415926

namespace BajaWildcatRacing
{

class DistCalcProcedure : public Procedure {
    public:

        DrivetrainSubsystem& drivetrainSubsystem;
        DashSubsystem& dashSubsystem;
        DataStorage& dataStorage;
        Coms& coms;

        float deltaTime = 0;
        float prevCarTime;
        // MPS = meters per second
        float prevCarMPS = 0;

        float distMeters = 0;

        //Counter so we only send to dash every 5 hz 
        int cycleNum = 0;

        DistCalcProcedure(DrivetrainSubsystem& drivetrainSubsystem, DashSubsystem& dashSubsystem, DataStorage& dataStorage, Coms& coms)
        : drivetrainSubsystem(drivetrainSubsystem)
        , dashSubsystem(dashSubsystem)
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

            float rpm = (drivetrainSubsystem.getFrontRightRPM()+drivetrainSubsystem.getFrontLeftRPM())/2;

            float mps = rpm * 0.0289f; // Magic number 

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

            //Avoid flooding the dash with can commands
            cycleNum++;
            if(cycleNum  % 12 == 0){
                dashSubsystem.sendDistance(distMeters * 0.000621371f);
            }

            // float rpm1 = drivetrainSubsystem.getFrontLeftRPM();
            // float rpm2 = drivetrainSubsystem.getFrontRightRPM();
            // float rpm3 = drivetrainSubsystem.getRearRPM();
            // std::cout << "RPM 1: " << rpm1 << std::endl;
            // std::cout << "RPM 2: " << rpm2 << std::endl;
            // std::cout << "RPM 3: " << rpm3 << std::endl;

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

