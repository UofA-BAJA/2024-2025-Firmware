#include "Dash.h"


namespace BajaWildcatRacing
{

    Dash::Dash(CANDispatcher& canDispatcher, byte deviceId) : CANDevice(canDispatcher, deviceId){
        indicatorLightState = 0;
    }


    // Sends latest speed information to the Dash
    void Dash::sendSpeed(float speed)
    {
        std::vector<byte> data(sizeof(float), 0);

        memcpy(data.data(), &speed, sizeof(float));

        sendLossyCanCommand(0x01, data);
        
    }


    // Sends latest RPM information to the Dash
    void Dash::sendRPM(float rpm)
    {
        std::vector<byte> data(sizeof(float), 0);

        memcpy(data.data(), &rpm, sizeof(float));

        sendLossyCanCommand(0x02, data);

    }

    // Sends latest CVT Temp information to the Dash
    void Dash::sendCVTTemp(float cvtTemp)
    {
        std::vector<byte> data(sizeof(float), 0);

        memcpy(data.data(), &cvtTemp, sizeof(float));
        
        sendLossyCanCommand(0x03, data);
        
    }

    // Sends latest Time Elapsed information to the Dash
    void Dash::sendTimeSeconds(float seconds)
    {
        std::vector<byte> data(sizeof(float), 0);
        
        memcpy(data.data(), &seconds, sizeof(float));
        
        sendLossyCanCommand(0x04, data);

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
        std::vector<byte> data(sizeof(uint16_t), 0);

        memcpy(data.data(), &this->indicatorLightState, sizeof(uint16_t));

        sendLossyCanCommand(0x05, data);
    }

    //Sends latest distance infromation to the dash
    void Dash::sendDistance(float distance){
        std::vector<byte> data(sizeof(float), 0);
        
        memcpy(data.data(), &distance, sizeof(float));
        
        sendLossyCanCommand(0x06, data);
    }
}
