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

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {});
}

// Sends latest speed information to the Dash
void Dash::sendSpeed(float speed)
{
    byte canID = Devices::Dash;
    std::vector<byte> data = {0x01, rotX};

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
    std::vector<byte> data = {0x02, rotX};

    if (!canDispatcher)
    {
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {});
}