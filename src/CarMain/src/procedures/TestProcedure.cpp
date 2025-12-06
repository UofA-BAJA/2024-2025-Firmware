#include "Procedure.h"
#include "TestSubsystem.h"


#include <iomanip>

namespace BajaWildcatRacing
{

class TestProcedure : public Procedure{
    public:
        TestSubsystem& testSubsystem;



        TestProcedure(TestSubsystem& testSubsystem)
        : testSubsystem(testSubsystem)
        {
            this->frequency = 120; 

        }
        
        void init() override {
            std::cout << "Test procedure initialized!" << std::endl;
        }

        void execute() override {

            
            TestDevice::TestStruct st = testSubsystem.getTestStruct();


            testSubsystem.test(true);
            // testSubsystem.test(false);
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << "Test Struct: " << st.a << " " << st.b << " " <<  st.c << " " << st.d << " " << st.e << std::endl;
        }

        void end() override {
            std::cout << "test procedure ended" << std::endl;
        }

        bool isFinished() override {
            return false;
        }

        std::string toString() override {
            return "Testing Procedure";
        }

    private:
};

}
