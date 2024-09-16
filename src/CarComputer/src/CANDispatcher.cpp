#include "CANDispatcher.h"



CANDispatcher::CANDispatcher(const char* interface){

    can_socket_fd = openCANSocket(interface);

    currUID = MIN_UID_BOUND;

    readCANInterface(can_socket_fd);
}

void CANDispatcher::sendCanCommand(int deviceID, std::vector<byte> data, std::function<void(can_frame)> callback){

    if(data.size() > 7){
        
        std::cerr << "Error: You are only allowed to send 7 bytes of data to CAN device." << std::endl;
        return;
    }

    uint16_t messageID = currUID + 1;                                                 // The unique messageID that CAN will send back to the PI to perform a callback

    if(messageID > MAX_UID_BOUND){
        messageID = MIN_UID_BOUND;
    }

    if(callbacks.find(messageID) == callbacks.end()){
        std::cerr << "Error: Sending CAN requests too fast! Slow down!" << std::endl;
        return;
    }


    // Prepare the CAN frame
    struct can_frame frame;                                             // The CAN frame to send to the CAN device
    frame.can_id = deviceID;                                            // CAN ID
    frame.can_dlc = data.size();                                        // Data length

    for(int i = 0; i < data.size(); i++){
        frame.data[i+1] = data.at(i);
    }

    // Send the CAN frame
    if(write(can_socket_fd, &frame, sizeof(frame)) != sizeof(frame)){
        std::cerr << "Error sending CAN frame: " << strerror(errno) << std::endl;
        return;
    }

    // Now when we receive a CAN frame with ID of message ID, we will trigger the callback.
    std::lock_guard<std::mutex> lock(callbacks_mutex);
    callbacks[messageID] = callback;
    std::cout << "CAN frame sent!" << std::endl;
}




void CANDispatcher::readCANInterface(int socket){
    std::thread([this, socket](){
        struct can_frame frame;

        // Continuous loop to read from the CAN bus
        while(true){
            int nbytes = read(socket, &frame, sizeof(struct can_frame));

            if(nbytes > 0){
                uint16_t message_id = frame.can_id;

                std::lock_guard<std::mutex> lock(callbacks_mutex);

                uint16_t callback_key = message_id;

                // Check to see if the can frame is actually meant for us.
                if(callbacks.find(callback_key) != callbacks.end()){
                    // Invoke the registered callback
                    callbacks[callback_key](frame);
                }
            }
        }
    }).detach();
}

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