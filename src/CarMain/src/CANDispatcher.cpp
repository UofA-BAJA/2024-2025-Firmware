#include "CANDispatcher.h"

namespace BajaWildcatRacing
{

    CANDispatcher::CANDispatcher(const char* interface){

        interfaceName = interface;

        // resetCANInterface(interfaceName);

        can_socket_fd = openCANSocket(interface);

        currUID = MIN_UID_BOUND;

        canReadingThread = std::thread(&CANDispatcher::readCANInterface, this);
    }



    void CANDispatcher::execute(){
        //TODO: this should be time based, rather than 6 per car cycle (i.e. decouple execute() from the car frequency)
        //Send 6 commands per cycle to avoid saturating the output buffer
        for(int i = 0; i < 6; i++){
            sendNextCanCommand();
        }
        
        std::lock_guard<std::mutex> lock(callbacks_mutex);

        //Iterate over all pending responses and drop ones over 100 cycles
        for(int i = 0; i < MAX_RESPONSE_ARRAY_BOUND; i++){

            //Only increment the cycles if it's the first UID 
            if(responses[i] != nullptr && ((responses[i]->firstUID) % MAX_RESPONSE_ARRAY_BOUND == i)){
                (responses[i]->commandCycles)++;
            }
            if(responses[i] != nullptr && (responses[i]->commandCycles) >= cycleThreshold){
                droppedCommands++;
                std::cout << "Commands Dropped: " << droppedCommands << std::endl;
                // std::cout << "Command Dropped: "  << (i) << std::endl;
                
                responses[i] = nullptr;
                
                //TODO: resend for dropped lossless command
            }
        } 

        // float droppedCommandRatio = (float) droppedCommands / totalCommands * 100.0;

        // std::cout << "Commands Dropped: " << droppedCommands << std::endl;
        // std::cout << std::fixed;
        // std::cout << std::setprecision(2);
        // std::cout << "Percentage dropped: " << droppedCommandRatio << std::endl;
        
        
    }

    /*
    *  Method:  sendCanCommand
    *
    *  Purpose: 
    *
    *  Pre-Condition:  There is a device on the CAN bus with ID deviceID; The size of the data
    *                  vector is 8 or less
    *
    *  Post-Condition: The data is successfully sent over the CAN bus to the device with deviceID;
    *                  When a message is received, the given callback function will be executed
    *
    *  Parameters:
    *          deviceID -- The ID of the device on the CAN bus to send the data to
    *              
    *          data -- The data to send to the device on the CAN bus that represents a command for the device
    * 
    *          destination -- The pointer to the data that the callback will write to
    * 
    *          callback -- A function to be executed once the device responds with data
    *
    *  Returns: None
    *
    */
    void CANDispatcher::sendCanRequest(int deviceCommandID, std::vector<byte> data, int recievedDataLength, std::function<void(void*)> callback){

        if(data.size() > 8){
            std::cerr << "Error: You are only allowed to send 8 bytes of data in a CAN frame." << std::endl;
            return;
        }

        CANCommand canCommand;
        canCommand.deviceCommandID = deviceCommandID;
        canCommand.data = data;
        canCommand.callback = callback;
        canCommand.recievedDataLength = recievedDataLength;
        canCommand.lossless = true; 

        queuedCommands.push(canCommand);
    }


    /*
    *  Method:  sendCanCommand
    *
    *  Purpose: Sends a CAN frame to a device connected to the CAN bus, where the data
    *           represents a command that the device should respond to. 
    * 
    *  Pre-Condition:  There is a device on the CAN bus with ID deviceID; The size of the data
    *                  vector is 8 or less
    *
    *  Post-Condition: The data is successfully sent over the CAN bus to the device with deviceID;
    *
    *  Parameters:
    *          deviceID -- The ID of the device on the CAN bus to send the data to
    *              
    *          data -- The data to send to the device on the CAN bus that represents a command for the device
    * 
    *  Returns: None
    *
    */
    void CANDispatcher::sendLossyCanCommand(int deviceCommandID, std::vector<byte> data){
        if(data.size() > 8){
            std::cerr << "Error: You are only allowed to send 8 bytes of data in a CAN frame." << std::endl;
            return;
        }

        CANCommand canCommand;
        canCommand.deviceCommandID = deviceCommandID;
        canCommand.data = data;
        canCommand.recievedDataLength = 0;
        canCommand.lossless = false;

        queuedCommands.push(canCommand);
    }

    void CANDispatcher::sendLosslessCanCommand(int deviceCommandID, std::vector<byte> data){
        if(data.size() > 8){
            std::cerr << "Error: You are only allowed to send 8 bytes of data in a CAN frame." << std::endl;
            return;
        }

        CANCommand canCommand;
        canCommand.deviceCommandID = deviceCommandID;
        canCommand.data = data;
        canCommand.recievedDataLength = 0;
        canCommand.lossless = true;

        queuedCommands.push(canCommand);
    }


    void CANDispatcher::sendNextCanCommand(){
    
        if(queuedCommands.empty()){
            return;
        }

        CANCommand canCommand = queuedCommands.front();
        queuedCommands.pop();

        // Get all the information about the CAN Command
        int deviceCommandID = canCommand.deviceCommandID;
        std::vector<byte> data = canCommand.data;
        std::function<void(void*)> callback = canCommand.callback;
        bool lossless = canCommand.lossless;
        int recievedDataLength = canCommand.recievedDataLength;


        // Prepare the actual CAN frame
        struct can_frame frame;                 // The CAN frame that will be sent
        frame.can_id = deviceCommandID << 20;   // CAN ID (deviceCommandID occupies the upper 9 bits)
        frame.can_id |= CAN_EFF_FLAG;           // Flag for SocketCAN to indicate extended CAN ID usage
        frame.can_dlc = data.size();            // Data length code (dlc)
        for(int i = 0; i < data.size(); i++){   // Copy data from vector to array
            frame.data[i] = data.at(i);         // (surely there's a better way to do this)
        }

        //Lossless command or request
        if(recievedDataLength > 0 || lossless == true){
            uint32_t messageID = currUID + 1;   // The unique messageID that the device will send back to the PI to perform a callback
    
            if(messageID > MAX_UID_BOUND){
                messageID = MIN_UID_BOUND; //Wraparound
            }
    
            currUID = messageID;

            //Update the CAN frame with the callback ID (or-ing it will include the lower 20 bits)
            frame.can_id |= currUID;

            //Prepare response object
            int numFrames = (recievedDataLength / 8) + 1; //8 bytes in a can frame, do integer division then add one because we have to round up
            std::shared_ptr<CANResponse> response(new CANResponse());
            response->firstUID = currUID; //We don't want the full can_id
            response->framesLeft = numFrames; 
            response->numFrames = numFrames;
            response->recievedDataLength = recievedDataLength; //Used to avoid segfaults from malformed frames
            response->callback = callback;
            response->commandCycles = 0;
            response->recievedData = std::make_unique<unsigned char[]>(recievedDataLength);
            response->deviceCommandID = deviceCommandID;

            //Only store sent data for lossless command (we need it if we need to resend)
            if(lossless){
                response->sentData = std::make_unique<unsigned char[]>(data.size());
                for(int i = 0; i < data.size(); i++){   // Copy data from vector to array
                    response->sentData[i] = data.at(i);         // (surely there's a better way to do this)
                }

            }

    
            // Thread safety (callbacks are handled in another thread)
            std::lock_guard<std::mutex> lock(callbacks_mutex);
            
            // This line doesn't seem to do anything at all
            // Supposedly it stops if we've wrapped over the available callback IDs but the callbacks at the end haven't been recieved or timed out
            if(responses[currUID % MAX_RESPONSE_ARRAY_BOUND] != nullptr){
                std::cerr << "Error: Sending CAN requests too fast! Slow down!" << std::endl;
                return;
            }            
            
            responses[currUID % MAX_RESPONSE_ARRAY_BOUND] = response;
            // std::cout << "reserved id " << currUID % MAX_RESPONSE_ARRAY_BOUND << std::endl;

            // (eventually) used for drop rate tracking & alerting
            // totalCommands++;

            //If we're expecting something super long back, reserve more callback IDs
            if(numFrames > 1){
                //Reserve more message IDs in a loop
                for(int i = 0; i < numFrames - 1; i++){
                    uint32_t messageID = currUID + 1;   // The unique messageID that the device will send back to the PI to perform a callback
    
                    if(messageID > MAX_UID_BOUND){
                        messageID = MIN_UID_BOUND; //Wraparound
                    }
                    currUID = messageID;
                    // std::cout << "reserved additional id " << currUID % MAX_RESPONSE_ARRAY_BOUND << std::endl;
                    responses[currUID % MAX_RESPONSE_ARRAY_BOUND] = response;
                }
            }
        }

        /* 
        * Note about extremely bizzare bug and the order of code execution:
        *
        * Very rarely, if we wrote to the can bus, the device would respond so quickly that
        * callbacks map and commandCycles wouldn't have time to be written to. To fix this, 
        * I just had to add them to those maps before the command is written over the can socket.
        * That's why the three lines are called before the write and not after.
        *    
        */

        // Send the CAN frame
        // std::cout << std::hex << frame.can_id << std::dec << std::endl; //Debugging print statement
        ssize_t result = write(can_socket_fd, &frame, sizeof(frame));

        // std::cout << result << std::endl;
        
        // Result is the length of the can frame, unless an error occurs
        if(result != sizeof(frame)){

            std::string errorStr = strerror(errno);
            std::cerr << "Error sending CAN frame: " << errorStr << std::endl;

            if(!errorStr.compare("No buffer space available")){
                CarLogger::LogWarning("CAN Buffer filled");
                resetCANInterface(interfaceName);
            }

            if(recievedDataLength > 0 || lossless == true){
                // Erase what we just wrote from the callbacks and commandCycles if it fails to send
                std::lock_guard<std::mutex> lock(callbacks_mutex); //Mutex is unlocked when it goes out of scope, need to relock
                uint32_t firstUID = responses[currUID % MAX_RESPONSE_ARRAY_BOUND]->firstUID;
                int numFrames = responses[currUID % MAX_RESPONSE_ARRAY_BOUND]->numFrames;
                for(int i = 0; i < numFrames; i++){
                    responses[(currUID + i) % MAX_RESPONSE_ARRAY_BOUND] = nullptr;
                }
            } 
        }

        //Mutex automatically gets unlocked when it goes out of scope
    }

    /*
    *  Method:  readCANInterface
    *
    *  Purpose: Continuously read the CAN bus and invoke registered callbacks stored in the callbacks map.
    *
    *  Pre-Condition:  This function is invoked on a separate thread so as to not block the main thread;
    * 
    *  Post-Condition: The CAN buffer is successfully read; When a CAN frame is read, the frame gets
    *                  forwarded via the callback method associated with the ID of the CAN frame;
    * 
    *  Parameters: None
    *
    *  Returns: None
    *
    */
    void CANDispatcher::readCANInterface(){

        struct can_frame frame;                     // The data read from the CAN bus will be stored here
        // Continuous loop to read from the CAN bus
        while(running.load()){

            ////////////////////////////////////////
            // Chat code
            // This code makes it so that the thread properly ends without blocking
            fd_set read_fds;
            struct timeval timeout;
    
            FD_ZERO(&read_fds);
            FD_SET(can_socket_fd, &read_fds);
    
            // Set timeout to 100ms (adjust as needed)
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000;
    
            // Wait for data on the socket with a timeout
            int retval = select(can_socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    
            if (retval < 0) {
                if (errno == EINTR) continue;  // Ignore interrupted system calls
                std::cerr << "Error in select(): " << strerror(errno) << std::endl;
                break;
            } else if (retval == 0) {
                // Timeout occurred, check if we need to exit
                continue;
            }
            // End of chat code
            ///////////////////////////////////////////////


            //Read the next frame from the CAN interface
            int nbytes = read(can_socket_fd, &frame, sizeof(struct can_frame));


            if (nbytes < 0) {
                if (errno == EBADF || errno == ECONNRESET) {
                    std::cout << "CAN interface shutting down." << std::endl;
                    break;
                }
                std::cerr << "ERROR reading CAN frame: " << strerror(errno) << std::endl;
                continue;
            }

            

            //TODO: Don't try to store stuff if it's a lossless *command*
            if(nbytes > 0){
                uint32_t messageID = frame.can_id & CAN_EFF_MASK; // AND to get only the 29-bit ID
                
                std::lock_guard<std::mutex> lock(callbacks_mutex);

                byte frameLength = (byte) frame.len;
                // Check to see if the can frame is actually meant for us.
                if(responses[messageID % MAX_RESPONSE_ARRAY_BOUND] != nullptr){
                    //Don't copy anything if we aren't expecting to copy anything
                    if(responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->recievedDataLength > 0){
                        uint32_t difference = messageID - responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->firstUID;

                        //**** Print Statements for debugging below
                        // std::cout << "messageID: " << std::hex << messageID << std::dec << " difference: " << difference << std::endl;
                        // std::cout << (int) frame.can_dlc << " " << frame.can_id << " " << (int) frame.len << " " << (int) frame.len8_dlc << std::endl;
                        // std::cout << "frame.len" << (int)frame.len << " frameLength " << (int)frameLength << std::endl;
                        // std::cout << "expected end point: " << difference*8 + frameLength << " max length: " << responses[messageID]->recievedDataLength << std::endl;

                        //If the data we're getting back exceeds the area allocated, error out. Segfault prevention.
                        if(difference*8 + frameLength > responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->recievedDataLength){
                            std::cerr << "ERROR: A response exceeded the area allocated for response data." << std::endl;
                        }else{
                            //Copy the frame data into the right place in the array
                            //*******Print statement for debugging
                            // std::cout << (int)frame.data[0] << " " << (int)frame.data[1] << " " << (int)frame.data[2] << " " << (int)frame.data[3] << std::endl;

                            memcpy(responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->recievedData.get() + (difference*8), frame.data, frameLength);

                            
                            responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->framesLeft--;
                            //Run the callback if we've gotten all the frames
                            if(responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->framesLeft == 0){
                                responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->callback(responses[messageID % MAX_RESPONSE_ARRAY_BOUND]->recievedData.get());
                            }

                        }
                    }
                    //Always erase the messageID from the list now that we've recieved it      
                    responses[messageID % MAX_RESPONSE_ARRAY_BOUND] = nullptr;
                }
            }
        }

        std::cout << "readCANInterface() has stopped." << std::endl;

    }

    void CANDispatcher::end(){

        

        running = false;
        
        if (canReadingThread.joinable()) {
            canReadingThread.join();  // Wait for the thread to finish
        }

        close(can_socket_fd);
    }

    /*
    *  Method:  openCANSocket
    *
    *  Purpose: Open a socket on the Raspberry PI that is used to interface with the CAN bus
    *
    *  Pre-Condition:  interface is a string with the name of an existing, active CAN interface
    * 
    *  Post-Condition: The socket is sucessfully opened and is readable and writeable.
    * 
    *  Parameters:
    *          interface -- A string that represents the name of the interface we will open a socket for
    *
    *  Returns: A socket file descriptor that is used to interface with the socket that was opened
    *
    */
    int CANDispatcher::openCANSocket(const char* interface){
        int socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);                  // socket returns a file descriptor for a socket
        if(socket_fd < 0){
            std::cerr << "Error while opening socket: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        struct ifreq ifr;                       // ifreq is used in networking to interact with network interfaces. In this case, the can0 interface.

        // Copy the interface str (can0) into ifr.name
        strcpy(ifr.ifr_name, interface);
        // ioctl (input and output control) is used to talk to device drivers. In this case, we are getting the network index of the interface and binding it to the socket. 
        if(ioctl(socket_fd, SIOGIFINDEX, &ifr) < 0){
            std::cerr << "Error getting interface index: " << strerror(errno) << std::endl;
            close(socket_fd);
            exit(EXIT_FAILURE);
        }

        struct sockaddr_can addr;
        memset(&addr, 0, sizeof(addr));         // Zero the struct to avoid junk values.
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if(bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
            std::cerr << "Error in socket bind: " << strerror(errno) << std::endl;
            close(socket_fd);
            exit(EXIT_FAILURE);
        }

        return socket_fd;
    }


    void CANDispatcher::resetCANInterface(const char* interface){


        std::string canDownCommand = "sudo ip link set " + std::string(interface) + " down";
        std::string canUpCommand = "sudo ip link set " + std::string(interface) + " up";

        int result1 = std::system(canDownCommand.c_str());

        if(result1 == 0){
            std::cout << "CAN Interface set down successfully" << std::endl;
        }

        int result2 = std::system(canUpCommand.c_str());

        if(result2 == 0){
            std::cout << "CAN Interface set up successfully" << std::endl;
        }

    }

}