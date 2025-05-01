#include "Procedure.h"
#include "DrivetrainSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "Coms.h"

#include <iomanip>

namespace BajaWildcatRacing
{

class TemperatureLoggingProcedure : public Procedure{
    public:
        DrivetrainSubsystem& drivetrainSubsystem;
        DataStorage& dataStorage;
        Coms& coms;


        TemperatureLoggingProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
        : drivetrainSubsystem(drivetrainSubsystem)
        , dataStorage(dataStorage)
        , coms(coms)
        {


            this->frequency = 30;

        }
        
        void init() override {
            std::cout << "Temperature procedure initialized!" << std::endl;
        }

        void execute() override {

            float cvt_temperature = drivetrainSubsystem.getCVTTemperature();

            coms.sendData(DataTypes::CVT_TEMPERATURE, cvt_temperature);

            dataStorage.storeData(temperature, DataTypes::TEMPERATURE);
            coms.sendData(DataTypes::CVT_TEMPERATURE, temperature);

            // std::cout << std::fixed;
            // std::cout << std::setprecision(2);
            // std::cout << "CVT Temperature: " << cvt_temperature << std::endl;
        }

        void end() override {
            std::cout << "Temperature procedure ended" << std::endl;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Temperature Procedure";
        }

    private:
};

}
