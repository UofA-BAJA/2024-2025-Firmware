#include "Procedure.h"
#include "IMUSubsystem.h"

#include <iomanip>

class IMUProcedure : public Procedure{
    public:
        IMUSubsystem* imuSubsystem;

        IMUProcedure(IMUSubsystem *imuSubsystem){
            this->imuSubsystem = imuSubsystem;
        }
        
        void init() override {
            // float xRot = imuSubsystem->getRotationX();
            // float yRot = imuSubsystem->getRotationY();
            // float zRot = imuSubsystem->getRotationZ();
            // xRot = imuSubsystem->getRotationX();
            // yRot = imuSubsystem->getRotationY();
            // zRot = imuSubsystem->getRotationZ();
            // xRot = imuSubsystem->getRotationX();
            // yRot = imuSubsystem->getRotationY();
            // zRot = imuSubsystem->getRotationZ();
            std::cout << "IMU procedure initialized!" << std::endl;
        }

        void execute() override {
            // std::cout << "IMU procedure execution"<< std::endl;

            float xRot = imuSubsystem->getRotationX();

            // For testing purposes
            xRot = imuSubsystem->getRotationX();
            xRot = imuSubsystem->getRotationX();
            xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            // xRot = imuSubsystem->getRotationX();
            float yRot = imuSubsystem->getRotationY();
            float zRot = imuSubsystem->getRotationZ();

            std::cout << std::fixed;
            std::cout << std::setprecision(0);
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