#ifndef TESTDEVICE_H
#define TESTDEVICE_H

#include "CANDevice.h"

namespace BajaWildcatRacing
{

    class TestDevice : CANDevice {

        public:

            TestDevice(CANDispatcher& candispatcher, byte deviceId);

            

            typedef struct TestStruct_s{
                float a;
                float b;
                float c;
                float d;
                float e;
            } TestStruct;


            TestStruct getTestStruct();
            void sendTest(bool lossless);

        private:
            TestStruct test;

    };

}

#endif