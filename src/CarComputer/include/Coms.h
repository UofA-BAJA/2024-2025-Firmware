#ifndef COMS_H
#define COMS_H

#include "Commands.h"
#include "ProcedureScheduler.h"
#include "LiveDataStream.h"
#include "CarLogger.h"

#include <iostream>

// For multithreading
#include <mutex>
#include <thread>
#include <chrono> // I believe this is used for the sleep function in the executeRadio loop

// Radio library
#include <RF24/RF24.h>


class Coms{

    struct DataPacket{
        int streamMask = 0;
        float timestamp;
        float data[6];
    };

    // State machine yay
    enum RadioState{
        TRANSMITTING,
        RECEIVING
    };

    enum PitCommandState{
        IDLE,
        LIVE_DATA_TRANSMIT,
        DATABASE_TRANSMIT
    };


    public:
        Coms(ProcedureScheduler* procedureScheduler);

        void addNewLiveDataStream(LiveDataStream* stream);

        void execute(float timestamp);

    private:
        const bool RADIO_ACTIVE = true;

        RadioState currentRadioState = RadioState::RECEIVING;
        PitCommandState currentPitCommandState = PitCommandState::LIVE_DATA_TRANSMIT;

        float currTimestamp;
        void executeRadio();
        void radioReceive();
        void radioTransmit();

        void transmitLiveData();
        void transmitDatabase();

        void tryUpdateState();

        const int maxPackets = 6;
        void sortPackets(DataPacket packets[]);

        ProcedureScheduler* procedureScheduler;
        LiveDataStream* liveDataStreams[32];

        int liveStreamCount = 0;

        std::mutex timestampMutex;



        std::mutex dataStreamMutex;
        std::thread radioThread;

};

#endif