#include "Procedure.h"
#include "CarLogger.h"
#include "DashSubsystem.h"

class DashProcedure : public Procedure{
    public:
        DashSubsystem* dashSubsystem;
        float testSpeed = 0.0f;
        float testRPM = 00.0f;

        DashProcedure(DashSubsystem *DashSubsystem){
            this->dashSubsystem = dashSubsystem;
            this->frequency = 2;
        }

        void init() override{
            std::cout << "Dash Procedure Initialized" << std::endl;
        }

        void execute() override {
            testSpeed += 0.5f;
            testRPM += 0.5f;
            dashSubsystem->sendRPM(testRPM);
            dashSubsystem->sendSpeed(testSpeed);
        }

        void end() override {
            testSpeed = 0.0f;
            testRPM = 0.0f;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Dash Procedure";
        }
    private:
};