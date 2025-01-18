#include "Procedure.h"
#include "CarLogger.h"
#include "IMUSubsystem.h"
#include "DashSubsystem.h"



class DashProcedure : public Procedure{
    public:

        DashSubsystem* dashSubsystem;
        IMUSubsystem* imuSubsystem;

        DashProcedure(DashSubsystem* dashSubsystem, IMUSubsystem* imuSubsystem){
            this->dashSubsystem = dashSubsystem;
            this->imuSubsystem = imuSubsystem;
            this->frequency = 20;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
            // dashSubsystem->sendRPM(testSpeed);
        }

        void execute() override {
            dashSubsystem->sendRPM(imuSubsystem->getRotationX());
            // dashSubsystem->sendTimeSeconds(Car::time);
        }

        void end() override {
            // testSpeed = 0.0f;
            // testRPM = 0.0f;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Dash Procedure";
        }
    private:
};