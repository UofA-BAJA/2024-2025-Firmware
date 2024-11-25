#include "Procedure.h"
#include "TemperatureSubsystem.h"
//#include "DataStorage.h"
#include "CarLogger.h"

#include <iomanip>

class TemperatureProcedure : public Procedure{
    public:
        TemperatureSubsystem* imuSubsystem;
        DataStorage* dataStorage;

        TemperatureProcedure(TemperatureSubsystem *temperatureSubsystem, DataStorage* dataStorage){
            this->temperatureSubsystem = temperatureSubsystem;
            this->dataStorage = dataStorage;

            this->frequency = 60;

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