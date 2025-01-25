#include "CANDevice.h"


void CANDevice::sendCanCommand(Device::Devices deviceID, byte commandByte, void* receivedData){

    using namespace std::chrono;


    // This will create a unique key for each device + command combination. That way the map can store
    // this information and use it to keep track of when the last time that command was sent to the device
    u_int16_t deviceCommandKey = (deviceID << 8) | (commandByte);


    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();

    // I think it should be fine without doing any initialization for the activeCommandTimes map. 
    // Needs to be tested though.
    
    double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = deviceID;
        std::vector<byte> data = {commandByte};


        if(!m_canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        m_canDispatcher->sendCanCommand(canID, data, receivedData, [this](can_frame frame, void* destination) {this ->populateValue(frame, destination);});
        activeCommandTimes[deviceCommandKey] = now;
    }
}



void CANDevice::populateValue(can_frame frame, void* destination){
    // I think all the data we'll be sending back is of size 4 and will be a float
    int size = 4;

    memcpy(destination, &frame.data, size);
}



void CANDevice::sendCanCommand(Device::Devices deviceID, byte commandByte, std::vector<byte> rawData){

    using namespace std::chrono;

    // This will create a unique key for each device + command combination. That way the map can store
    // this information and use it to keep track of when the last time that command was sent to the device
    u_int16_t deviceCommandKey = (deviceID << 8) | (commandByte);


    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();

    // I think it should be fine without doing any initialization for the activeCommandTimes map. 
    // Needs to be tested though.
    
    double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();

    if(timeDifference > minimumRepeatThreshold){
        byte canID = deviceID;

        if(!m_canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        m_canDispatcher->sendCanCommand(canID, rawData);
        activeCommandTimes[deviceCommandKey] = now;
    }
}