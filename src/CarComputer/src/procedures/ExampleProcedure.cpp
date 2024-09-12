#include <iostream>
#include <string>
#include "Procedure.h"


class ExampleProcedure : public Procedure {
    public:


        // ExampleSubsystem exampleSubsystem;
        
        void init(){
            std::cout << "Example procedure initialized!" << std::endl;
        }

        void execute(){
            std::cout << "Example procedure execution: " << i << std::endl;
            i++;
        }

        void end(){
            std::cout << "Example procedure ended: " << i << std::endl;
            i++;
        }

        bool isFinished(){
            return (i > 500);
        }

        std::string toString(){
            return "Example Procedure";
        }

    private:
        int i = 0;

};
