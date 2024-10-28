#ifndef DASH_H
#define DASH_H

#include "CANDispatcher.h"

class Dash
{
public:
    Dash(CANDispatcher *canDispatcher);

    void sendSpeed(float speed);
    void sendRPM(float rpm);

    // for testing only
    void sendIMURotX(float rotX);

private:
    CANDispatcher *canDispatcher;
}

#endif