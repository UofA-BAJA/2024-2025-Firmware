#ifndef CANDEVICE_H
#define CANDEVICE_H

#include "CANDispatcher.h"
#include "Devices.h"
#include "Constants.h"


#include <functional>
#include <chrono>
#include <map>


class CANDevice{

    private:
        CANDispatcher* m_canDispatcher;
        double minimumRepeatThreshold;

        std::unordered_map<uint16_t, std::chrono::steady_clock::time_point> activeCommandTimes;

        void populateValue(can_frame frame, void* destination);



    public:
        CANDevice(CANDispatcher* canDispatcher) : m_canDispatcher(canDispatcher){
            this->minimumRepeatThreshold = 1/BASE_CAR_FREQUENCY * 1000;
        }

    protected:
        void sendCanCommand(Devices deviceID, byte commandByte, void* receivedData);
        void sendCanCommand(Devices deviceID, byte commandByte);

};




#endif