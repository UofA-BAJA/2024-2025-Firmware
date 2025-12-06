/*
 *
 *  Class: CANDevice 
 *
 *         Author:  Matthew Larson  
 *
 *        Purpose:  Class for wrapper libraries to inherit from to help perform CAN communication to their devices;
 *                  Handles interaction with the CANDispatcher; handles spam protection (each command can only send once per car cycle);
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
 *  Inst. Methods:  void sendCanRequest(deviceID, commandByte, recievedData) -- send CAN command with callback to populate recievedData
 *                  void sendCanCommand(deviceID, commandByte, rawData) -- send CAN command with raw data (no callback)
 *        (private) void populateValue(frame, destination) -- populate destination with the CAN frame's data, should only be called by a callback
 *
*/

#include "CANDevice.h"

namespace BajaWildcatRacing
{

    //TODO: FIX DOCUMENTATION!!!!!!!!!
    /*
    *   Method: sendCanRequest (with data)
    *
    *   Purpose: Send CAN request and data to a device, and data will be recieved back
    *
    *   Pre-conditions:  
    *   
    *   Post-condition:  The CAN command is sent to the device with the given deviceID. When the data is recieved,
    *   receivedData is populated with the callback data
    *
    *   @param commandByte: The byte representing the type of the data to request from the device !!!4 bits!!!
    *   @parama data: Up to 8 bytes of data to send to the CAN device
    *   @param recievedData: A pointer to a location where the recieved data will be written once recieved.
    *   @param recievedDataLength: Number representing the length of recieved data.
    *
    *   @returns None
    *
    */
    void CANDevice::sendCanRequest(byte dataType, std::vector<byte> data, void* dataDestination, int dataDestinationLength){

        using namespace std::chrono;

        if((dataType & ~(0b1111)) > 0){
            std::cerr << "Error: dataType is out of range (>4 bits)." << std::endl;
            return;
        }


        //This is the unique identifier for device + datatype that is used in the CAN ID for the CANT Protocol.
        //This is also used as a key in the activeCommandTimes map to keep track of when each command was last sent.
        u_int16_t deviceCommandKey = (deviceId << 4) | (dataType);


        //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
        steady_clock::time_point now = steady_clock::now();
        double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();
        if(timeDifference > minimumRepeatThreshold){
            auto callback = [this, dataDestination, dataDestinationLength](void* recievedData){
                this->populateValue(recievedData, dataDestination, dataDestinationLength);
            };
            m_canDispatcher.sendCanRequest(deviceCommandKey, data, dataDestinationLength, callback);
            activeCommandTimes[deviceCommandKey] = now;
        }
    }

    //Overload without data
    void CANDevice::sendCanRequest(byte dataType, void* dataDestination, int dataDestinationLength){
        //Initialize an empty vector and call the overload
        std::vector<byte> data;
        sendCanRequest(dataType, data, dataDestination, dataDestinationLength);
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
    void CANDevice::populateValue(void* recievedData, void* dataDestination, int dataDestinationLength){
        // I think all the data we'll be sending back is of size 4 and will be a float
        // The above comment is left in memorial of the ignorance of us in 2024-2025
        memcpy(dataDestination, recievedData, dataDestinationLength);
    }



    /*
    *   Method: sendLossyCanCommand (with data)
    *
    *   Purpose: Send a lossy CAN command to a device with data. No data is returned and there is no verification of reception
    *   
    *   Pre-conditions: 
    *   
    *   Post-condition: The CAN command is sent to the device with the given deviceID.  
    *
    *   @param deviceID: The CAN ID of the device to send the command to
    *   @param commandByte: The byte representing the type of the data being sent to the device.
    *   @param rawData: The data to send to the device. At most 8 bytes.    
    *
    *   @returns None
    */
    void CANDevice::sendLossyCanCommand(byte dataType, std::vector<byte> data){

        using namespace std::chrono;

        if((dataType & ~(0b1111)) > 0){
            std::cerr << "Error: dataType is out of range (>4 bits)." << std::endl;
            return;
        }


        //This is the unique identifier for device + datatype that is used in the CAN ID for the CANT Protocol.
        //This is also used as a key in the activeCommandTimes map to keep track of when each command was last sent.
        u_int16_t deviceCommandKey = (deviceId << 4) | (dataType);


        //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
        steady_clock::time_point now = steady_clock::now();
        double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();
        if(timeDifference > minimumRepeatThreshold){
            m_canDispatcher.sendLossyCanCommand(deviceCommandKey, data);
            activeCommandTimes[deviceCommandKey] = now;
        }
    }

    //No extra data overload
    void CANDevice::sendLossyCanCommand(byte dataType){
        //Initialize an empty vector and call the overload
        std::vector<byte> data;
        sendLossyCanCommand(dataType, data);
    }


    /*
    *   Method: sendLosslessCanCommand (with data)
    *
    *   Purpose: Send a lossless CAN command to a device with data. No data is returned.
    *   
    *   Pre-conditions: 
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
    void CANDevice::sendLosslessCanCommand(byte dataType, std::vector<byte> data){

        using namespace std::chrono;

        if((dataType & ~(0b1111)) > 0){
            std::cerr << "Error: dataType is out of range (>4 bits)." << std::endl;
            return;
        }


        //This is the unique identifier for device + datatype that is used in the CAN ID for the CANT Protocol.
        //This is also used as a key in the activeCommandTimes map to keep track of when each command was last sent.
        u_int16_t deviceCommandKey = (deviceId << 4) | (dataType);


        //If this is being called quicker than the minimum repeat threshold, don't send a new CAN command
        steady_clock::time_point now = steady_clock::now();
        double timeDifference = duration_cast<milliseconds>(now-activeCommandTimes[deviceCommandKey]).count();
        if(timeDifference > minimumRepeatThreshold){
            activeCommandTimes[deviceCommandKey] = now;
            m_canDispatcher.sendLosslessCanCommand(deviceCommandKey, data);
        }
    }

    //No data overload
    void CANDevice::sendLosslessCanCommand(byte dataType){
        //Initialize an empty vector and call the overload
        std::vector<byte> data;
        sendLosslessCanCommand(dataType, data);
    }
    
}
