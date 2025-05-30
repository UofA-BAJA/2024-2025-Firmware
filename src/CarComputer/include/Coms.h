#ifndef COMS_H
#define COMS_H

#include "Commands.h"
#include "ProcedureScheduler.h"
#include "LiveDataStream.h"
#include "CarLogger.h"

#include "Commands.h"
#include "Constants.h"

#include <iostream>

// For multithreading
#include <mutex>
#include <thread>
#include <atomic>

#include <chrono> // I believe this is used for the sleep function in the executeRadio loop
#include <bitset> // No idea what this is for, but I think it's important for something
#include <unordered_set>
#include <memory>

// Radio library
#include <RF24/RF24.h>

namespace BajaWildcatRacing
{

    class Coms{

        struct DataPacket{
            int streamMask = 0;
            float timestamp;
            float data[6];
        };


        enum PitCommandState{
            IDLE,
            LIVE_DATA_TRANSMIT,
            DATABASE_TRANSMIT
        };


        public:
            Coms(ProcedureScheduler& procedureScheduler);


            void execute(float timestamp);
            void end();

            void sendData(DataTypes dataType, float data);


        private:

            // This radio should only be accessed from the radioThread
            // We do not want a mutex to protect it, as it would bee very slow
            // and defeat the purpose of multithreading
            RF24 radio;

            void addNewLiveDataStream(std::shared_ptr<LiveDataStream> stream);
            
            const bool RADIO_ACTIVE = true;

            PitCommandState currentPitCommandState = PitCommandState::IDLE;

            float currTimestamp = 0;
            void executeRadio();
            void radioTransmit();

            void transmitLiveData();
            void transmitDatabase();
            void idle();

            void tryUpdateState();

            const int maxPackets = 6;
            void sortPackets(DataPacket packets[]);

            ProcedureScheduler& procedureScheduler;
            std::shared_ptr<LiveDataStream> liveDataStreams[32];
            std::unordered_map<DataTypes, std::shared_ptr<LiveDataStream>> liveDataStreamMap;

            int liveStreamCount = 0;

            std::mutex timestampMutex;
            std::mutex procedureSchedulerMutex;



            std::mutex dataStreamMutex;
            std::thread radioThread;
            std::atomic<bool> running = RADIO_ACTIVE;

    };

}


#endif
