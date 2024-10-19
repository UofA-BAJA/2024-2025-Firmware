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

using byte = unsigned char;

class CANDispatcher{

    public:

        CANDispatcher(const char* interface);

        void execute();

        void sendCanCommand(int deviceID, std::vector<byte> data, std::function<void(can_frame)> callback);

    private:
        const byte MIN_UID_BOUND = 0xA0;
        const byte MAX_UID_BOUND = 0xFF;

        int can_socket_fd;
        byte currUID;

        std::thread canReadingThread;

        std::map<byte, std::function<void(can_frame)>> callbacks;
        // Maps a command to the amount of cycles it has been waiting for a response
        std::map<byte, int> commandCycles;
        int cycleThreshold = 40;     // A command can be in queue for 5 cycles until it is considered dropped.

        const char* interfaceName;

        std::mutex callbacks_mutex;

        int openCANSocket(const char* interface);
        void readCANInterface();

        void resetCANInterface(const char* interface);
        int droppedCommands = 0;

};


#endif