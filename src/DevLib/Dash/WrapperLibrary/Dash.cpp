#include "Dash.h"

Dash::Dash(CANDispatcher *canDispatcher){
    this->canDispatcher = canDispatcher;
    this->indicatorLightState = 0;
}


// Sends latest speed information to the Dash
void Dash::sendSpeed(float speed)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(sizeof(float)+1, 0);

    memcpy(data.data()+1, &speed, sizeof(float));
    data[0] = 0x01;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}


// Sends latest RPM information to the Dash
void Dash::sendRPM(float rpm)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(sizeof(float)+1, 0);

    memcpy(data.data()+1, &rpm, sizeof(float));
    data[0] = 0x02;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sends latest CVT Temp information to the Dash
void Dash::sendCVTTemp(float cvtTemp)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(sizeof(float)+1, 0);

    memcpy(data.data()+1, &cvtTemp, sizeof(float));
    data[0] = 0x03;
    

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sends latest Time Elapsed information to the Dash
void Dash::sendTimeSeconds(float seconds)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(sizeof(float)+1, 0);
    
    memcpy(data.data()+1, &seconds, sizeof(float));
    data[0] = 0x04;
    
    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sets new indicator light state, all indicator lights at once 
// Each bit represents 1 light, least significant is light 0
void Dash::setIndicatorLights(uint16_t lightState)
{
    this->indicatorLightState = lightState;
}

// Sets new indicator light state for a specific indicator light
void Dash::setSpecificIndicatorLight(IndicatorLights light, bool state){
    if(state){
        this->indicatorLightState = this->indicatorLightState | 1 << light;
    }else{
        this->indicatorLightState = this->indicatorLightState & ~(1 << light);
    }
} 

// Sends the current indicator light state in the wrapper library to the dash
// This should ONLY be called by the dash procedure at the end of its cycle
void Dash::sendIndicatorLightState(){
    byte canID = Devices::DASH;
    std::vector<byte> data(sizeof(uint16_t)+1, 0);

    memcpy(data.data()+1, &this->indicatorLightState, sizeof(uint16_t));
    data[0] = 0x05;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}