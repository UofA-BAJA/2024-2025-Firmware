#include "Procedure.h"
#include "DrivetrainSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "LiveDataStream.h"
#include "Coms.h"

#include <iomanip>

namespace BajaWildcatRacing
{

class TemperatureProcedure : public Procedure{
    public:
        DrivetrainSubsystem& drivetrainSubsystem;
        DataStorage& dataStorage;
        Coms& coms;

        // LiveDataStream* temperatureStream;


        TemperatureProcedure(DrivetrainSubsystem& drivetrainSubsystem, DataStorage& dataStorage, Coms& coms)
        : drivetrainSubsystem(drivetrainSubsystem)
        , dataStorage(dataStorage)
        , coms(coms)
        {

            // temperatureStream = new LiveDataStream(DataTypes::CVT_TEMPERATURE);

            // coms.addNewLiveDataStream(temperatureStream);

            this->frequency = 1;

        }
        
        void init() override {
            std::cout << "Temperature procedure initialized!" << std::endl;
        }

        void execute() override {

            float cvt_temperature = drivetrainSubsystem.getCVTTemperature();

            // temperatureStream->enqueue(cvt_temperature);

            //dataStorage->storeData(temperature, DataTypes::TEMPERATURE);

            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << "CVT Temperature: " << cvt_temperature << std::endl;
        }

        void end() override {
            // ! Remember to reset any local variables! This class does not actually get destroyed, only reused!
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
