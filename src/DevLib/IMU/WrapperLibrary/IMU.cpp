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
    std::vector<byte> data = {0x01};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationX(frame);});

    return rotX;
}

void IMU::populateRotationX(can_frame frame){
    memcpy(&rotX, &frame.data, sizeof(rotX));
}


float IMU::getLatestRotationY(){
    
    byte canID = Devices::IMU;
    std::vector<byte> data = {0x02};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationY(frame);});

    return rotY;
}

void IMU::populateRotationY(can_frame frame){
    memcpy(&rotY, &frame.data, sizeof(rotY));
}

float IMU::getLatestRotationZ(){

    byte canID = Devices::IMU;
    std::vector<byte> data = {0x03};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationZ(frame);});

    return rotZ;
}

void IMU::populateRotationZ(can_frame frame){
    memcpy(&rotZ, &frame.data, sizeof(rotZ));
}