#include "Procedure.h"
#include "IMUSubsystem.h"
#include "DataStorage.h"
#include "CarLogger.h"
#include "Coms.h"

#include "LiveDataStream.h"

#include <iomanip>

class IMUProcedure : public Procedure{
    public:
        IMUSubsystem* imuSubsystem;
        DataStorage* dataStorage;
        LiveDataStream* xRotStream;
        Coms* coms;

        IMUProcedure(IMUSubsystem *imuSubsystem, DataStorage* dataStorage, Coms* coms){
            this->imuSubsystem = imuSubsystem;
            this->dataStorage = dataStorage;
            this->coms = coms;

            xRotStream = new LiveDataStream(DataTypes::IMU_ROTATION_X);

            coms->AddNewStream(xRotStream);

            this->frequency = 20;

        }
        
        void init() override {
            std::cout << "IMU procedure initialized!" << std::endl;
        }

        void execute() override {
            // std::cout << "IMU procedure execution"<< std::endl;

            // For testing purposes
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();

            float xRot = imuSubsystem->getRotationX();
            float yRot = imuSubsystem->getRotationY();
            float zRot = imuSubsystem->getRotationZ();

            float xAccel = imuSubsystem->getAccelerationX();
            float yAccel = imuSubsystem->getAccelerationY();
            float zAccel = imuSubsystem->getAccelerationZ();

            dataStorage->storeData(xRot, DataTypes::IMU_ROTATION_X);
            dataStorage->storeData(yRot, DataTypes::IMU_ROTATION_Y);
            dataStorage->storeData(zRot, DataTypes::IMU_ROTATION_Z);

            dataStorage->storeData(xAccel, DataTypes::IMU_ACCELERATION_X);
            dataStorage->storeData(yAccel, DataTypes::IMU_ACCELERATION_Y);
            dataStorage->storeData(zAccel, DataTypes::IMU_ACCELERATION_Z);

            xRotStream->enqueue(xRot);

            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << "X: " << xRot << " Y: " << yRot << " Z: " << zRot << std::endl;
            std::cout << "X-A: " << xAccel << " Y-A: " << yAccel << " Z-A: " << zAccel << std::endl;
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