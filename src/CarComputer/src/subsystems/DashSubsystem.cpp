#include "DashSubsystem.h"

DashSubsystem::DashSubsystem(CANDispatcher* canDispatcher){
    this->dash = new Dash(canDispatcher);

}


void DashSubsystem::sendSpeed(float speed){

    dash->sendSpeed(speed);
}

void DashSubsystem::sendRPM(float rpm){
    dash->sendRPM(rpm);
}

void DashSubsystem::sendCVTTemp(float cvtTemp){

    dash->sendCVTTemp(cvtTemp);
}

void DashSubsystem::sendTimeSeconds(unsigned long seconds){
    dash->sendTimeSeconds(seconds);
}

void DashSubsystem::setIndicatorLights(uint16_t lightState){
    dash->setIndicatorLights(lightState);
}