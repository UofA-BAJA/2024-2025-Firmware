/*
 *
 *  Class: CANDevice 
 *
 *         Author:  Matthew Larson  
 *
 *        Purpose: 
 *
 *  Inherits From:  None
 *
 *     Interfaces:  None
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:  None
 *
 *+-----------------------------------------------------------------------
 *
 *   Constructors:  CANDevice(CANDispatcher*) -- Takes the CANDispatcher to be used. 
 *                      Also sets the minimum threshold for a callback-type command to be repeated based on Constants::BASE_CAR_FREQUENCY
 *
 *  Class Methods:  None
 *
 *  Inst. Methods:  void sendCanCommand(deviceID, commandByte, recievedData) -- send CAN command with callback to populate recievedData
 *                  void sendCanCommand(deviceID, commandByte, rawData) -- send CAN command with raw data (no callback)
 *        (private) void populateValue(frame, destination) -- populate destination with the CAN frame's data, should only be called by a callback
 *
*/

#include "CANDevice.h"


/*
*   Method: sendCanCommand (Callback overload)
*
*   Purpose: Send CAN command to a device with a callback to populate data
*
*   Pre-conditions:  There is a device on the CAN bus with ID deviceID;
*                    commandByte is a valid command for the device;
*                    recievedData is the correct data type for the data to be recieved
*   
*   Post-condition:  The CAN command is sent to the device with the given deviceID. When the data is recieved,
*   receivedData is populated with the callback data
*
*   @param deviceID: The CAN ID of the device to send the command to
*   @param commandByte: The byte representing the type of the data to request from the device
*   @param recievedData: A pointer to a location where the recieved data will be written once recieved.   
*
*   @returns None
*
*/
void CANDevice::sendCanCommand(Device::Devices deviceID, byte commandByte, void* receivedData){

    using namespace std::chrono;


    // This will create a unique key for each device + command combination. That way the map can store
    // this information and use it to keep track of when the last time that command was sent to the device
    u_int16_t deviceCommandKey = (deviceID << 8) | (commandByte);


    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();

    // I think it should be fine without doing any initialization for the activeCommandTimes map. 
    // Needs to be tested though.
    
    double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();
    if(timeDifference > minimumRepeatThreshold){
        byte canID = deviceID;
        std::vector<byte> data = {commandByte};


        if(!m_canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        m_canDispatcher->sendCanCommand(canID, data, receivedData, [this](can_frame frame, void* destination) {this ->populateValue(frame, destination);});
        activeCommandTimes[deviceCommandKey] = now;
    }
}


/*
*  Method: populateValue (private)
*
*  Purpose: Populate a value when it is recieved from the CAN device.
*
*  Pre-condition:   frame has valid data (TEMP: MUST BE SIZE 4) 
*                   destination is a valid pointer (TEMP: MUST BE SIZE 4)
*
*  @param frame: The CAN frame recieved from the device
*  @param destination: A pointer to the location where the recieved data will be written.
*
*  @returns None
*/
void CANDevice::populateValue(can_frame frame, void* destination){
    // I think all the data we'll be sending back is of size 4 and will be a float
    int size = 4;

    memcpy(destination, &frame.data, size);
}



/*
*   Method: sendCanCommand (No callback overload)
*
*   Purpose: Send CAN command to a device with data. The device cannot return anything.
*   
*   Pre-conditions: There is a device on the CAN bus with ID deviceID.
*                   rawData is at most 8 bytes.
*   
*   Post-condition: The CAN command is sent to the device with the given deviceID.  
*
*   @param deviceID: The CAN ID of the device to send the command to
*   @param commandByte: The byte representing the type of the data being sent to the device.
*   **USED FOR TRACKING ONLY - YOU MUST INCLUDE AN IDENTIFYING BYTE IN rawData IF ONE IS NEEDED BY THE DEVICE**
*   @param rawData: The data to send to the device. At most 8 bytes.    
*
*   @returns None
*/
void CANDevice::sendCanCommand(Device::Devices deviceID, byte commandByte, std::vector<byte> rawData){

    using namespace std::chrono;

    // This will create a unique key for each device + command combination. That way the map can store
    // this information and use it to keep track of when the last time that command was sent to the device
    u_int16_t deviceCommandKey = (deviceID << 8) | (commandByte);


    //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
    steady_clock::time_point now = steady_clock::now();

    // I think it should be fine without doing any initialization for the activeCommandTimes map. 
    // Needs to be tested though.
    
    double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();

    if(timeDifference > minimumRepeatThreshold){
        byte canID = deviceID;

        if(!m_canDispatcher){
            std::cerr << "Error: CANDispatcher is null!" << std::endl;
        }

        m_canDispatcher->sendCanCommand(canID, rawData);
        activeCommandTimes[deviceCommandKey] = now;
    }
}