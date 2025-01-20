#include "Procedure.h"
#include "TemperatureSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "LiveDataStream.h"
#include "Coms.h"

#include <iomanip>

class TemperatureProcedure : public Procedure{
    public:
        TemperatureSubsystem* temperatureSubsystem;
        DataStorage* dataStorage;
        Coms* coms;

        LiveDataStream* temperatureStream;


        TemperatureProcedure(TemperatureSubsystem *temperatureSubsystem, DataStorage* dataStorage, Coms* coms){
            this->temperatureSubsystem = temperatureSubsystem;
            this->dataStorage = dataStorage;
            this->coms = coms;

            temperatureStream = new LiveDataStream(DataTypes::CVT_TEMPERATURE);

            coms->addNewLiveDataStream(temperatureStream);

            this->frequency = 1;

        }
        
        void init() override {
            std::cout << "Temperature procedure initialized!" << std::endl;
        }

        void execute() override {
            // std::cout << "IMU procedure execution"<< std::endl;

            // For testing purposes
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();

            float cvt_temperature = temperatureSubsystem->getTemperature();

            temperatureStream->enqueue(cvt_temperature);

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