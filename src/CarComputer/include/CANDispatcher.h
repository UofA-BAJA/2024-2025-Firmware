#ifndef CANDISPATCHER_H
#define CANDISPATCHER_H

#include <iostream>
#include <linux/can.h>
#include <sys/socket.h>
#include <net/if.h>
#include <stdint.h>
#include <map>
#include <functional>
#include <cstring>      // For strerror()
#include <cstdlib>      // For exit()
#include <unistd.h>     // For close()
#include <sys/ioctl.h>

#include <iomanip>

// For multithreading
#include <thread>
#include <mutex>

#include <vector>

#include "CarLogger.h"
#include "CarTime.h"

namespace BajaWildcatRacing
{

    using byte = unsigned char;

    class CANDispatcher{

        public:

            CANDispatcher(const char* interface);

            void execute();

            void sendCanCommand(int deviceID, std::vector<byte> data, void* destination, std::function<void(can_frame, void*)> callback);
            void sendCanCommand(int deviceID, std::vector<byte> data);

        private:
            const int MIN_UID_BOUND = 0x1000;         // Reserve the non extended ID's for all the other devices
            const int MAX_UID_BOUND = 0xFFFFFF;

            int can_socket_fd;
            uint32_t currUID;

            std::thread canReadingThread;

            std::unordered_map<uint32_t, std::function<void(can_frame, void*)>> callbacks;
            std::unordered_map<uint32_t, void*> destinations;
            // Maps a command to the amount of cycles it has been waiting for a response
            std::unordered_map<uint32_t, int> commandCycles;
            int cycleThreshold = 100;     // A command can be in queue for 100 cycles until it is considered dropped.

            const char* interfaceName;

            std::mutex callbacks_mutex;

            int openCANSocket(const char* interface);
            void readCANInterface();

            void resetCANInterface(const char* interface);
            unsigned long droppedCommands = 0;
            unsigned long totalCommands = 0;

    };

}



#endif