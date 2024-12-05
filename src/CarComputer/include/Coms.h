#ifndef COMS_H
#define COMS_H

#include "Commands.h"
#include "ProcedureScheduler.h"
#include "LiveDataStream.h"

// For multithreading
#include <mutex>
#include <thread>
#include <chrono>

class Coms{

    struct DataPacket{
        int streamMask;
        float timestamp;
        float data[6];
    };

    public:
        Coms(ProcedureScheduler* procedureScheduler);

        void AddNewStream(LiveDataStream* stream);

        void execute(float timestamp);

    private:

        float currTimestamp;

        void executeRadio();

        ProcedureScheduler* procedureScheduler;
        LiveDataStream* liveDataStreams[32];

        int liveStreamCount = 0;

        std::mutex dataStreamMutex;
        std::thread radioThread;

};

#endif