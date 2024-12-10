#include "Procedure.h"
#include "CarLogger.h"
#include "DashSubsystem.h"

class DashProcedure : public Procedure{
    DashSubsystem* dashSubsystem;

    DashProcedure(DashSubsystem *DashSubsystem){
        this->dashSubsystem = dashSubsystem;
        this->frequency = 5;
    }

    void init() override{
        std::cout << "Dash Procedure Initialized" << std::endl;
    }

    void execute() override {
        
    }

    void end() override {

    }

    bool isFinished() override {
        return false;
    }

    std::string toString() override {
            return "Dash Procedure";
        }
};