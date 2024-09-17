
#include <iostream>
#include <cstring>      // For strerror()
#include <cstdlib>      // For exit()
#include <unistd.h>     // For close()
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

int open_can_socket(const char* interface){

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

void send_can_message(int socket_fd){
    // Prepare the CAN frame
    struct can_frame frame;
    frame.can_id = 0x123;   // CAN ID
    frame.can_dlc = 3;      // Data length
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0xFF;


    // Send the CAN frame
    if(write(socket_fd, &frame, sizeof(frame)) != sizeof(frame)){
        std::cerr << "Error sending CAN frame: " << strerror(errno) << std::endl;
    }
    else{
        std::cout << "CAN frame sent!" << std::endl;
    }
}

void receive_can_message(int socket_fd){
    struct can_frame frame;

    // Wait for and read the CAN frame
    ssize_t nbytes = read(socket_fd, &frame, sizeof(frame));
    if(nbytes < 0){
        std::cerr << "Error receiving CAN frame: " << strerror(errno) << std::endl;
        return;
    }

    // Output the received frame
    std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
    std::cout << "Data: ";
    for(int i = 0; i < frame.can_dlc; i++){
        std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
    }

    std::cout << std::endl;
}



int main(){

    // This assumes there's a network device named can0
    // Use the following command to verify the operation of the device.
    // ip link show can0 
    const char *can_interface = "can0";

    int socket_fd = open_can_socket(can_interface);

    send_can_message(socket_fd);

    return 0;
}