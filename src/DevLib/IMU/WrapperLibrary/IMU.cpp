#include "Devices.h"
#include "IMU.h"


IMU::IMU(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
}

double IMU::requestAccelerationX(){

}

double IMU::requestAccelerationY(){

}

double IMU::requestAccelerationZ(){

}

double IMU::requestRotationX(){

    byte canID = Devices::IMU;
    std::vector<byte> data = {0xAB};


    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->getRotationX(frame);});


    // struct can_frame frame;
    // receive_can_message(can_socket_fd, frame);

}

void IMU::getRotationX(can_frame frame){

    // Output the received frame
    std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
    std::cout << "Data: ";
    for(int i = 0; i < frame.can_dlc; i++){
        std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
    }

    std::cout << std::endl;
}


double IMU::requestRotationY(){
    
}

double IMU::requestRotationZ(){
    
}

// ssize_t receive_can_message(int socket_fd, struct can_frame &frame){
//     // Wait for and read the CAN frame
//     ssize_t nbytes = read(socket_fd, &frame, sizeof(frame));
//     if(nbytes < 0){
//         std::cerr << "Error receiving CAN frame: " << strerror(errno) << std::endl;
//         return;
//     }

//     // // Output the received frame
//     // std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
//     // std::cout << "Data: ";
//     // for(int i = 0; i < frame.can_dlc; i++){
//     //     std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
//     // }

//     // std::cout << std::endl;
// }