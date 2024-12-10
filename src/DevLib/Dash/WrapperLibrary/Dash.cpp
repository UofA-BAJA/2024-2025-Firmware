#include "Dash.h"

Dash::Dash(CANDispatcher *canDispatcher)
{
    this->canDispatcher = canDispatcher;
}


// FOR TESTING PURPOSES ONLY, if we actually manage to run this
void Dash::sendIMURotX(float rotX)
{
    byte canID = Devices::Dash;
    std::vector<byte> data = {0x45, rotX};

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {});
}

// Sends latest speed information to the Dash
void Dash::sendSpeed(float speed)
{
    byte canID = Devices::Dash;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, speed, sizeof(float));
    data[0] = 0x01;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {});
}

// Sends latest RPM information to the Dash
void Dash::sendRPM(float rpm)
{
    byte canID = Devices::Dash;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, rpm, sizeof(float));
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
    byte canID = Devices::Dash;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, cvtTemp, sizeof(float));
    data[0] = 0x03;
    

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sends latest Time Elapsed information to the Dash
void Dash::sendTimeSeconds(unsigned long seconds)
{
    byte canID = Devices::Dash;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, seconds, sizeof(unsigned long));
    data[0] = 0x04;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}

// Sends new indicator light state to the Dash
void Dash::sendTimeSeconds(uint16_t lightState)
{
    byte canID = Devices::Dash;
    std::vector<byte> data(8, 0);

    memcpy(data.data()+1, lightState, sizeof(uint16_t));
    data[0] = 0x05;

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data);
}