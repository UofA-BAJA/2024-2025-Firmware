#ifndef COMS_H
#define COMS_H

#include "Commands.h"
#include "ProcedureScheduler.h"
#include "LiveDataStream.h"
#include "CarLogger.h"

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

        void AddNewStream(LiveDataStream* stream);

        void execute(float timestamp);

    private:

        const bool RADIO_ACTIVE = true;
        // This radio should only be accessed from the radioThread
        // We do not want a mutex to protect it, as it would bee very slow
        // and defeat the purpose of multithreading
        RF24 radio(CE_PIN, CSN_PIN);
        RadioState currentRadioState = RadioState::RECEIVING;
        PitCommandState currentPitCommandState = PitCommandState::IDLE;

        float currTimestamp;
        void executeRadio();
        void radioReceive();
        void radioTransmit();

        void transmitLiveData();
        void transmitDatabase();

        void tryUpdateState();

        ProcedureScheduler* procedureScheduler;
        LiveDataStream* liveDataStreams[32];

        int liveStreamCount = 0;

        std::mutex timestampMutex;



        std::mutex dataStreamMutex;
        std::thread radioThread;

};

#endif