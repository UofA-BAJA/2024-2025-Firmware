#include <iostream>
#include <string>
#include "Procedure.h"

#include "subsystems/DataStorage.h"

class TestProcedure : public Procedure {
    public:

        DataStorage exampleSubsystem;

        TestProcedure(DataStorage &dataSubsystem){
            this->exampleSubsystem = dataSubsystem;
        }

        void init() override {
            std::cout << "Test procedure initialized!" << std::endl;
        }

        void execute() override {
            std::cout << "Test procedure execution: " << i << std::endl;
            i++;

            exampleSubsystem.storeData(i);
            exampleSubsystem.getData();
        }

        void end() override {
            // ! Remember to reset any local variables! This class does not actually get destroyed, only reused!
            i = 0;
            std::cout << "Test procedure ended" << std::endl;
        }

        bool isFinished() override {
            return (i > 500);
        }

        std::string toString() override {
            return "Test Procedure";
        }

    private:
        int i = 0;

};
