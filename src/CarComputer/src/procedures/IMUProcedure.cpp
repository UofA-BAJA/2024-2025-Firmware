#include "Procedure.h"
#include "IMUSubsystem.h"

class IMUProcedure : public Procedure{
    public:
        IMUSubsystem* imuSubsystem;

        IMUProcedure(IMUSubsystem *imuSubsystem){
            this->imuSubsystem = imuSubsystem;
        }
        
        void init() override {
            std::cout << "IMU procedure initialized!" << std::endl;
        }

        void execute() override {
            // std::cout << "IMU procedure execution"<< std::endl;

            float xRot = imuSubsystem->getRotationX();

            std::cout << "IMU Rotation X: " << xRot << std::endl;
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