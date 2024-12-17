#include "Devices.h"
#include "Temperature.h"
#include "Constants.h"

Temperature::Temperature(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
    this->minimumRepeatThreshold = 1/BASE_CAR_FREQUENCY * 1000;
}

float Temperature::getLatestTemperature(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastTemperatureTime).count();

    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::CVT_TEMP;
        std::vector<byte> data = {0x01};

        if(!canDispatcher){
          std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateTemperature(frame);});
        lastTemperatureTime = now;
    }
    
    return temperature;
}

void Temperature::populateTemperature(can_frame frame){
    memcpy(&temperature, &frame.data, sizeof(temperature));
}