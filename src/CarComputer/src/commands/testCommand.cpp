#include <iostream>
#include <string>
#include "Command.h"

class TestCommand : public Command {
    public:
        void init(){
            std::cout << "Test command initialized!" << std::endl;
        }
        void execute(){
            std::cout << "Execution! " << i << std::endl;
            i++;
        }
        std::string toString(){
            return "Test Command";
        }
    private:
        int i = 0;

};
