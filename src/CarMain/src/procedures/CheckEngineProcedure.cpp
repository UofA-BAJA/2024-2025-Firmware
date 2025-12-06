#include "Procedure.h"
#include "DashSubsystem.h"
#include "Dash.h"

namespace BajaWildcatRacing
{
    class CheckEngineProcedure : public Procedure{
        public:
            DashSubsystem& dashSubsystem;
    
    
            CheckEngineProcedure(DashSubsystem& dashSubsystem)
            : dashSubsystem(dashSubsystem)
            {
    
                this->frequency = 1;
    
            }

            void init() override {
                dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CHECK_ENGINE, true);
                std::cout << "Check Engine Light enabled!!" << std::endl;
            }

            void execute() override{

            }

            void end() override {
                // ! Remember to reset any local variables! This class does not actually get destroyed, only reused!
                dashSubsystem.setSpecificIndicatorLight(Dash::IndicatorLights::CHECK_ENGINE, false);
                std::cout << "Check Engine Light disabled!!" << std::endl;
            }
    
            bool isFinished() override {
                return false;
            }
    
            std::string toString() override {
                return "Check Engine Light Procedure";
            }
        private:
    };
}