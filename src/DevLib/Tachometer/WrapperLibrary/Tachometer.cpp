

#include "Tachometer.h"


Tachometer::Tachometer(CANDispatcher* canDispatcher) : CANDevice(canDispatcher) {

}

float Tachometer::getEngineRPM(){
    sendCanCommand(Device::TACHOMETER, 0x01, &engineRPM);
    return engineRPM;
}