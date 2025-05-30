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
        
        std::lock_guard<std::mutex> lock(callbacks_mutex);

        for(auto it = commandCycles.begin(); it != commandCycles.end();){
            uint32_t commandID = it->first;

            commandCycles[commandID]++;

            if(commandCycles[commandID] >= cycleThreshold){
                    droppedCommands++;
                    std::cout << "Commands Dropped: " << droppedCommands << std::endl;
                    // std::out << "Command Dropped: " << std::hex << commandID << std::endl;
                    callbacks.erase(commandID);
                    destinations.erase(commandID);
                    // Proper way to continue iterating over the map
                    it = commandCycles.erase(it);
            }
            else{
                ++it;
            }
        } 

        float droppedCommandRatio = (float) droppedCommands / totalCommands * 100.0;

        // std::cout << "Commands Dropped: " << droppedCommands << std::endl;
        // std::cout << std::fixed;
        // std::cout << std::setprecision(2);
        // std::cout << "Percentage dropped: " << droppedCommandRatio << std::endl;
        
    }

    /*
    *  Method:  sendCanCommand
    *
    *  Purpose: Sends a CAN frame to a device connected to the CAN bus, where the data
    *           represents a command that the device should respond to. If we expect a
    *           response from the device, provide a function that will get called when
    *           a response is received.
    *
    *  Pre-Condition:  There is a device on the CAN bus with ID deviceID; The size of the data
    *                  vector is 4 or less
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
    void CANDispatcher::sendCanCommand(int deviceID, std::vector<byte> data, void* destination, std::function<void(can_frame, void*)> callback){

        if(data.size() > 4){
            
            std::cerr << "Error: You are only allowed to send 4 bytes of data to CAN device." << std::endl;
            return;
        }

        uint32_t messageID = currUID + 1;   // The unique messageID that the device will send back to the PI to perform a callback


        // std::cout << currUID << std::endl;

        if(messageID > MAX_UID_BOUND){
            messageID = MIN_UID_BOUND;
        }

        currUID = messageID;


        byte callbackID[3];
        callbackID[0] = (currUID >> 0) & 0xff;
        callbackID[1] = (currUID >> 8) & 0xff;
        callbackID[2] = (currUID >> 16) & 0xff;


        std::lock_guard<std::mutex> lock(callbacks_mutex);


        // This line doesn't seem to do anything at all.
        if(callbacks.find(currUID) != callbacks.end()){
            std::cerr << "Error: Sending CAN requests too fast! Slow down!" << std::endl;
            return;
        }


        // Prepare the CAN frame
        struct can_frame frame = {};                                        // The CAN frame to send to the CAN device
        frame.can_id = deviceID;                                            // CAN ID
        frame.can_id |= CAN_EFF_FLAG;
        // dlc stands for data length code. It is plus 3 because we are sending the data and the callback
        frame.can_dlc = data.size()+3;
        frame.data[0] = callbackID[0];
        frame.data[1] = callbackID[1];
        frame.data[2] = callbackID[2];

        for(int i = 0; i < data.size(); i++){
            frame.data[i+3] = data.at(i);
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

        // Now when we receive a CAN frame with ID of message ID, we will trigger the callback.
        callbacks[currUID] = callback;
        destinations[currUID] = destination;
        commandCycles[currUID] = 0;

        ssize_t result = write(can_socket_fd, &frame, sizeof(frame));
        totalCommands++;


        // std::cout << result << std::endl;

        // Send the CAN frame
        if(result != sizeof(frame)){

            std::string errorStr = strerror(errno);
            std::cerr << "Error sending CAN frame: " << errorStr << std::endl;
            // Erase what we just wrote from the callbacks and commandCycles
            callbacks.erase(currUID);
            destinations.erase(currUID);
            commandCycles.erase(currUID);

            if(!errorStr.compare("No buffer space available")){
                CarLogger::LogWarning("CAN Buffer filled");
                resetCANInterface(interfaceName);
            }

            // exit(1);
        }
    }

    /*
    *  Method:  sendCanCommand
    *
    *  Purpose: Sends a CAN frame to a device connected to the CAN bus, where the data
    *           represents a command that the device should respond to. 
    * 
    *  Pre-Condition:  There is a device on the CAN bus with ID deviceID; The size of the data
    *                  vector is 4 or less
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

    void CANDispatcher::sendCanCommand(int deviceID, std::vector<byte> data){
        //*****UNTESTED
        if(data.size() > 8){
            
            std::cerr << "Error: You are only allowed to send 8 bytes of data to CAN device." << std::endl;
            return;
        }


        // Prepare the CAN frame
        struct can_frame frame;                                             // The CAN frame to send to the CAN device
        frame.can_id = deviceID;                                            // CAN ID
        frame.can_id |= CAN_EFF_FLAG;
        // dlc stands for data length code. 
        frame.can_dlc = data.size();
        for(int i = 0; i < data.size(); i++){
            frame.data[i] = data.at(i);
        }
        
        // Send the CAN frame
        ssize_t result = write(can_socket_fd, &frame, sizeof(frame));

        // std::cout << result << std::endl;
        
        if(result != sizeof(frame)){

            std::string errorStr = strerror(errno);
            std::cerr << "Error sending CAN frame: " << errorStr << std::endl;

            if(!errorStr.compare("No buffer space available")){
                CarLogger::LogWarning("CAN Buffer filled");
                resetCANInterface(interfaceName);
            }

            // exit(1);
        }
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



            int nbytes = read(can_socket_fd, &frame, sizeof(struct can_frame));

            if (nbytes < 0) {
                if (errno == EBADF || errno == ECONNRESET) {
                    std::cout << "CAN interface shutting down." << std::endl;
                    break;
                }
                std::cerr << "Error reading CAN frame: " << strerror(errno) << std::endl;
                continue;
            }

            if(nbytes > 0){


                uint32_t messageID = frame.can_id & CAN_EFF_MASK; // Mask to get only the 29-bit ID

                std::lock_guard<std::mutex> lock(callbacks_mutex);

                // Check to see if the can frame is actually meant for us.
                if(callbacks.find(messageID) != callbacks.end()){
                    // Invoke the registered callback and pass the destination variable
                    callbacks[messageID](frame, destinations[messageID]);
                    
                    callbacks.erase(messageID);
                    destinations.erase(messageID);
                    commandCycles.erase(messageID);
                }
            }
        }

        std::cout << "Read CAN interface thread out of scope" << std::endl;

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
            std::cout << "Can down executed successfully" << std::endl;
        }

        int result2 = std::system(canUpCommand.c_str());

        if(result2 == 0){
            std::cout << "Can up executed successfully" << std::endl;
        }

    }

}