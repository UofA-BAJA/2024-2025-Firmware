#ifndef DASHSUBSYSTEM_H
#define DASHSUBSYSTEM_H

#include "Dash.h"
#include "CANDispatcher.h"


class DashSubsystem {
    public:
        DashSubsystem(CANDispatcher* canDispatcher);
        void sendSpeed(float speed);
        void sendRPM(float rpm);
        void sendCVTTemp(float cvtTemp);
        void sendTimeSeconds(unsigned long seconds);
        void setIndicatorLights(uint16_t lightState);
    

    private:
        Dash* dash;    
};


#endif