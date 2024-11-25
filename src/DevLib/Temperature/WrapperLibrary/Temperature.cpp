#include "Devices.h"
#include "Temperature.h"

Temperature::Temperature(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
}

float Temperature::getLatestTemperature(){

    byte canID = Devices::CVT_TEMP;
    std::vector<byte> data = {0x01};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationZ(frame);});

    return temperature;
}

void Temperature::populateTemperature(can_frame frame){
    memcpy(&temp, &frame.data, sizeof(temperature));
}