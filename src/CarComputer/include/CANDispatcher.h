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

// For multithreading
#include <thread>
#include <mutex>

#include <vector>

using byte = unsigned char;

class CANDispatcher{

    public:

        CANDispatcher(const char* interface);

        void sendCanCommand(int deviceID, std::vector<byte> data, std::function<void(can_frame)> callback);

    private:
        const int MIN_UID_BOUND = 0;
        const int MAX_UID_BOUND = 255;

        int can_socket_fd;
        int currUID;

        std::thread canReadingThread;

        std::map<uint16_t, std::function<void(can_frame)>> callbacks;
        std::map<int, uint16_t> currentDeviceCommands;
        std::mutex callbacks_mutex;

        int openCANSocket(const char* interface);
        void readCANInterface();

        int droppedCommands = 0;

};


#endif