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
#include <atomic>

#include <queue>
#include <vector>
#include <memory>

#include "CarLogger.h"
#include "CarTime.h"

namespace BajaWildcatRacing
{

    using byte = unsigned char;

    class CANDispatcher{

        public:

            CANDispatcher(const char* interface);

            void execute();
            void end();

            void sendCanRequest(int deviceCommandID, std::vector<byte> data, int recievedDataLength, std::function<void(void*)> callback);
            void sendLossyCanCommand(int deviceCommandID, std::vector<byte> data);
            void sendLosslessCanCommand(int deviceCommandID, std::vector<byte> data);

        private:
            const int MIN_UID_BOUND = 0x00000001;  //0 is reserved for flagging a "no callback" condition     
            const int MAX_UID_BOUND = 0x000FFFFF;  //Lower 20 bits of CAN IDs are always used for callback IDs

            int can_socket_fd;
            uint32_t currUID;

            std::thread canReadingThread;
            std::atomic<bool> running = true;

            // Stores each response that we're waiting for (may be multiple CAN frames)
            typedef struct CANResponse{
                uint32_t firstUID; //Reference to the first UID in the chain, if it's a multi-frame response
                std::unique_ptr<unsigned char[]> recievedData; //Array that stores data recieved from the device 
                std::unique_ptr<unsigned char[]> sentData; //Array that stores data sent to device, to be resent if this is a lossless command
                int recievedDataLength; //Expected length of recieved data
                int numFrames; //Expected number of recieved frames
                int framesLeft; //Frames left to recieve 
                std::function<void(void*)> callback; //Callback function (called when all frames are recieved)
                int deviceCommandID; //The original command ID (upper 9 bits), used if we need a resend
                int commandCycles; //Number of "cycles" the command has been waiting for a response
            } CANResponse;

            //Response storage. Maps using modulo (so every 65535 sent commands, it wraps around)
            const static int MAX_RESPONSE_ARRAY_BOUND = 0xFFFF;
            std::shared_ptr<CANResponse> responses[MAX_RESPONSE_ARRAY_BOUND];
            
            
            int cycleThreshold = 100;     // A command can be in queue for 100 cycles until it is considered dropped.

            const char* interfaceName;

            std::mutex callbacks_mutex;

            int openCANSocket(const char* interface);
            void readCANInterface();

            // Pulls from the queue of commands and sends them accordingly
            void sendNextCanCommand();

            void resetCANInterface(const char* interface);
            unsigned long droppedCommands = 0;
            unsigned long totalCommands = 0;
            
            //Internal struct used for defining how a command should be dealt with 
            typedef struct CANCommand{
                int deviceCommandID; //This isn't a full CAN ID yet
                std::vector<byte> data;
                //Request: recievedDataLength > 0
                //Lossless command: receivedDataLength == 0, lossless = false
                //Lossy command: receivedDataLength == 0, lossless = true
                std::function<void(void*)> callback; //Callback is only called when all response frames are recieved
                int recievedDataLength;
                bool lossless;
            } CANCommand;

            std::queue<CANCommand> queuedCommands;

    };

}



#endif