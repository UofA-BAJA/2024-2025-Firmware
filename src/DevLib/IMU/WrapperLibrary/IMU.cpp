#include "Devices.h"
#include "IMU.h"
#include "Constants.h"


IMU::IMU(CANDispatcher* canDispatcher){
    this->canDispatcher = canDispatcher;
    this->minimumRepeatThreshold = 1/BASE_CAR_FREQUENCY * 1000;
}

float IMU::getLatestAccelerationX(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastAccXTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x04};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationX(frame);});
        lastAccXTime = now;
    }
    return accX;
}

void IMU::populateAccelerationX(can_frame frame){
    memcpy(&accX, &frame.data, sizeof(accX));
}

float IMU::getLatestAccelerationY(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastAccYTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x05};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationY(frame);});
        lastAccYTime = now;
    }
    return accY;
}

void IMU::populateAccelerationY(can_frame frame){
    memcpy(&accY, &frame.data, sizeof(accY));
}

float IMU::getLatestAccelerationZ(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastAccZTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x06};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateAccelerationZ(frame);});
        lastAccZTime = now;
    }
    return accZ;
}

void IMU::populateAccelerationZ(can_frame frame){
    memcpy(&accZ, &frame.data, sizeof(accZ));
}


float IMU::getLatestRotationX(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastRotXTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x01};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationX(frame);});
        lastRotXTime = now;
    }
    return rotX;
}

void IMU::populateRotationX(can_frame frame){
    memcpy(&rotX, &frame.data, sizeof(rotX));
}


float IMU::getLatestRotationY(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastRotYTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x02};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationY(frame);});
        lastRotYTime = now;
    }
    return rotY;
}

void IMU::populateRotationY(can_frame frame){
    memcpy(&rotY, &frame.data, sizeof(rotY));
}

float IMU::getLatestRotationZ(){
    using namespace std::chrono;

    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();
    double timeDifference = duration_cast<milliseconds>(now-lastRotZTime).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = Devices::IMU;
        std::vector<byte> data = {0x03};


        if(!canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        canDispatcher->sendCanCommand(canID, data, [this](can_frame frame) {this ->populateRotationZ(frame);});
        lastRotZTime = now;
    }
    return rotZ;
}

void IMU::populateRotationZ(can_frame frame){
    memcpy(&rotZ, &frame.data, sizeof(rotZ));
}