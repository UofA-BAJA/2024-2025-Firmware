#include <iostream>
#include <cstring>      // For strerror()
#include <cstdlib>      // For exit()
#include <unistd.h>     // For close()
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>


#include "DataTypes.h"
#include "IMU.h"


IMU::IMU(int can_socket_fd, int can_id){
    this->can_socket_fd = can_socket_fd;
    this->can_id = can_id;
}

double IMU::getAccelerationX(){

}

double IMU::getAccelerationY(){

}

double IMU::getAccelerationZ(){

}

double IMU::getRotationX(){
    send_can_message(can_socket_fd, can_id);

    // While the data is not the correct type, skip it.
    while(!peek_can_message(can_socket_fd, DataTypes::IMU_ROTATION_X)){
        ;
    }

    struct can_frame frame;
    receive_can_message(can_socket_fd, frame);

    // Output the received frame
    std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
    std::cout << "Data: ";
    for(int i = 0; i < frame.can_dlc; i++){
        std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
    }

    std::cout << std::endl;
}

double IMU::getRotationY(){
    
}

double IMU::getRotationZ(){
    
}


void send_can_message(int socket_fd, int can_id){
    // Prepare the CAN frame
    struct can_frame frame;
    frame.can_id = can_id;   // CAN ID
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

// Will return true if the can message contains data about dataType, and false if it does not.
bool peek_can_message(int socket_fd, DataTypes dataType){
    struct can_frame frame;

    int flags = 0;
    flags = MSG_PEEK;

    recv(socket_fd, &frame, sizeof(frame), flags);

    if(frame.can_id == dataType){
        return true;
    }

    return false;
    
}

ssize_t receive_can_message(int socket_fd, struct can_frame &frame){
    // Wait for and read the CAN frame
    ssize_t nbytes = read(socket_fd, &frame, sizeof(frame));
    if(nbytes < 0){
        std::cerr << "Error receiving CAN frame: " << strerror(errno) << std::endl;
        return;
    }

    // // Output the received frame
    // std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
    // std::cout << "Data: ";
    // for(int i = 0; i < frame.can_dlc; i++){
    //     std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
    // }

    // std::cout << std::endl;
}