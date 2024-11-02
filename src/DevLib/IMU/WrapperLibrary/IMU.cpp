#include "Devices.h"
#include "IMU.h"


IMU::IMU(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
}

float IMU::getLatestAccelerationX(){
    byte canID = Devices::IMU;
    std::vector<byte> data = {0x04};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationX(frame);});

    return accX;
}

void IMU::populateAccelerationX(can_frame frame){
    memcpy(&accX, &frame.data, sizeof(accX));
}

float IMU::getLatestAccelerationY(){
    byte canID = Devices::IMU;
    std::vector<byte> data = {0x05};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationY(frame);});

    return accY;
}

void IMU::populateAccelerationY(can_frame frame){
    memcpy(&accY, &frame.data, sizeof(accY));
}

float IMU::getLatestAccelerationZ(){
    byte canID = Devices::IMU;
    std::vector<byte> data = {0x06};


    if(!canDispatcher){
        std::cerr << "Error: CANDispatcher is null!" << std::endl;
    }

    canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationZ(frame);});

    return accZ;
}

void IMU::populateAccelerationZ(can_frame frame){
    memcpy(&accZ, &frame.data, sizeof(accZ));
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