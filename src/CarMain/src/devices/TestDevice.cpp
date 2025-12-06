#include "TestDevice.h"

namespace BajaWildcatRacing
{

    TestDevice::TestDevice(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId) {
        test.a = 0;
        test.b = 0;
        test.c = 0;
        test.d = 0;
        test.e = 0;

    }

    TestDevice::TestStruct TestDevice::getTestStruct(){
        sendCanRequest(0x01, &test, sizeof(TestStruct));
        return test;
    }

    void TestDevice::sendTest(bool lossless){
        if(lossless){
            sendLosslessCanCommand(0x02);
        }else{
            sendLossyCanCommand(0x02);
        }
    }

}