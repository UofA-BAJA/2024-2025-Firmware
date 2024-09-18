#include "Devices.h"
#include "IMU.h"


IMU::IMU(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
}

float IMU::getLatestAccelerationX(){
    return accX;
}

float IMU::getLatestAccelerationY(){
    return accY;
}

float IMU::getLatestAccelerationZ(){
    return accZ;
}


float IMU::getLatestRotationX(){

    byte canID = Devices::IMU;
    std::vector<byte> data = {0xA4, 0x84};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationX(frame);});

    return rotX;
}

void IMU::populateRotationX(can_frame frame){

    // Output the received frame
    // std::cout << "Received CAN frame with ID: 0x" << std::hex << frame.can_id << std::endl;
    // std::cout << "Data: ";
    // for(int i = 0; i < frame.can_dlc; i++){
    //     std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
    // }

    memcpy(&rotX, &frame.data, sizeof(rotX));

    // std:: cout << "X Rotation: " << rotX;
    // std::cout << std::endl;
}


float IMU::getLatestRotationY(){
    
    // Send command to CANDispatcher

    return rotY;
}

void IMU::populateRotationY(can_frame frame){
    memcpy(&rotY, &frame.data, sizeof(rotY));
}

float IMU::getLatestRotationZ(){

    // Send command to CANDispatcher

    return rotZ;
}

void IMU::populateRotationZ(can_frame frame){
    memcpy(&rotZ, &frame.data, sizeof(rotZ));
}