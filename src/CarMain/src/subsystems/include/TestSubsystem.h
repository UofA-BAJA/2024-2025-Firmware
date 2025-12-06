#ifndef TESTSUBSYSTEM_H
#define TESTSUBSYSTEM_H

#include "TestDevice.h"
#include "CANDispatcher.h"

namespace BajaWildcatRacing
{

    class TestSubsystem{
        public:
            TestSubsystem(CANDispatcher& canDispatcher);
            TestDevice::TestStruct getTestStruct();

            void test(bool lossless);

        private:
            TestDevice dev;
    };

}


#endif