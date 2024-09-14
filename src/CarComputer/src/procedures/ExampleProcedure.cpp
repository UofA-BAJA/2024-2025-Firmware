#include <iostream>
#include <string>
#include "Procedure.h"
#include "subsystems/DataStorage.h"

class ExampleProcedure : public Procedure {
    public:
        DataStorage exampleSubsystem;

        ExampleProcedure(DataStorage &dataStorage){
            exampleSubsystem = dataStorage;
        }
        
        void init() override {
            std::cout << "Example procedure initialized!" << std::endl;
        }

        void execute() override {
            std::cout << "Example procedure execution: " << i << std::endl;
            i++;

            exampleSubsystem.storeData(i);
            exampleSubsystem.getData();
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
