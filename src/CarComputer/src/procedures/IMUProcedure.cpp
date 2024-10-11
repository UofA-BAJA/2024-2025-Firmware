#include "Procedure.h"
#include "IMUSubsystem.h"
#include "DataStorage.h"

#include <iomanip>

class IMUProcedure : public Procedure{
    public:
        IMUSubsystem* imuSubsystem;
        DataStorage* dataStorage;

        IMUProcedure(IMUSubsystem *imuSubsystem, DataStorage* dataStorage){
            this->imuSubsystem = imuSubsystem;
            this->dataStorage = dataStorage;
        }
        
        void init() override {
            std::cout << "IMU procedure initialized!" << std::endl;
        }

        void execute() override {
            // std::cout << "IMU procedure execution"<< std::endl;

            float xRot = imuSubsystem->getRotationX();

            // For testing purposes
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            float yRot = imuSubsystem->getRotationY();
            float zRot = imuSubsystem->getRotationZ();


            dataStorage->storeData(xRot, DataTypes::IMU_ROTATION_X);
            dataStorage->storeData(yRot, DataTypes::IMU_ROTATION_Y);
            dataStorage->storeData(zRot, DataTypes::IMU_ROTATION_Z);

            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << "X: " << xRot << " Y: " << yRot << " Z: " << zRot << std::endl;
        }

        void end() override {
            // ! Remember to reset any local variables! This class does not actually get destroyed, only reused!
            std::cout << "IMU procedure ended" << std::endl;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "IMU Procedure";
        }

    private:
};