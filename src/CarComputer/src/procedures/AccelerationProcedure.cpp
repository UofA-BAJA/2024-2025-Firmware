#include <iostream>
#include <string>
#include "Procedure.h"

namespace BajaWildcatRacing
{

class AccelerationProcedure : public Procedure {
    public:

        AccelerationProcedure(){
        }
        
        void init() override {
            std::cout << "Example procedure initialized!" << std::endl;
        }

        void execute() override {
            std::cout << "Example procedure execution: " << i << std::endl;
            i++;

        }

        void end() override {
            // ! Remember to reset any local variables! This class does not actually get destroyed, only reused!
            i = 0;
            std::cout << "Example procedure ended" << std::endl;
        }

        bool isFinished() override {
            return (i > 2000);
        }

        std::string toString() override {
            return "Example Procedure";
        }

    private:
        int i = 1000;

};

}

