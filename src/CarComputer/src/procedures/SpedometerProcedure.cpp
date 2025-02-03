#include "Procedure.h"

#include "DrivetrainSubsystem.h"

namespace BajaWildcatRacing
{

class SpedometerProcedure : public Procedure {
    public:

        DrivetrainSubsystem& drivetrainSubsystem;

        SpedometerProcedure(DrivetrainSubsystem& drivetrainSubsystem)
        : drivetrainSubsystem(drivetrainSubsystem)
        {
            this->frequency = 10;
        }
        
        void init() override {
            std::cout << "Spedometer procedure initialized!" << std::endl;
        }

        void execute() override {

            std::cout << drivetrainSubsystem.getEngineRPM() << std::endl;

        }

        void end() override {
            std::cout << "Spedometer procedure ended" << std::endl;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Example Procedure";
        }

    private:

};

}

