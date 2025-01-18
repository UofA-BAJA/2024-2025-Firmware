#include "Dash.h"

Dash::Dash(CANDispatcher *canDispatcher){
    this->canDispatcher = canDispatcher;
}


// Sends latest speed information to the Dash
void Dash::sendSpeed(float speed)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(8, 0);

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
    std::vector<byte> data(8, 0);

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
    std::vector<byte> data(8, 0);

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
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, &seconds, sizeof(float));
    data[0] = 0x04;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sends new indicator light state to the Dash
void Dash::setIndicatorLights(uint16_t lightState)
{
    byte canID = Devices::DASH;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, &lightState, sizeof(uint16_t));
    data[0] = 0x05;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}