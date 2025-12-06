#include "CANTProtocol.h"
#define DEBUG_CAN 0

/*
    begin() - Call this to start recieving and responding to CAN frames

    @returns true if initialization succeeded, false if the inititalization failed due to: 
        - Invalid device ID
        - Failure to communicate to CAN chip (check wiring)
*/
bool CANTProtocol::begin(){
    //Check for valid CAN ID
    if(CAN_DEVICE_ID & ~(0b11111) > 0){
        return false;
    }

    //Begin CAN
    byte canInitResult = CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ);
    
    if(canInitResult != CAN_OK){
        return false;
    }
    #if DEBUG_CAN
        Serial.println(CAN_DEVICE_ID << 24, BIN);
    #endif
    //Filter to just the commands we want
    CAN.init_Mask(0, 1, 0x1F000000);
    CAN.init_Filt(0, 1, CAN_DEVICE_ID << 24);
    CAN.init_Filt(1, 1, CAN_DEVICE_ID << 24);

    CAN.init_Mask(1, 1, 0x1F000000);
    CAN.init_Filt(2, 1, CAN_DEVICE_ID << 24);
    CAN.init_Filt(3, 1, CAN_DEVICE_ID << 24);
    CAN.init_Filt(4, 1, CAN_DEVICE_ID << 24);
    CAN.init_Filt(5, 1, CAN_DEVICE_ID << 24);


    canInitResult = CAN.setMode(MCP_NORMAL);
    if(canInitResult != MCP2515_OK){
        return false;
    }

    pinMode(CAN_INTERRUPT_PIN, INPUT);

    //having to pass in this as a parameter to the function sucks!!!!!!
    attachInterrupt(digitalPinToInterrupt(CAN_INTERRUPT_PIN), ISRHandler, LOW, this);
    return true;
}


bool CANTProtocol::registerRequest(byte commandID, void (*onRecieved) (unsigned char dataLength, byte* incomingData, unsigned long callbackID)){
    //Check for valid command ID
    if(commandID & ~(0b1111) > 0){
        return false;
    }
    
    registeredMessages[commandID].onRecieved = onRecieved;
    registeredMessages[commandID].isRequest = true;
    registeredMessages[commandID].exists = true;

    return true;
}

bool CANTProtocol::registerCommand(byte commandID, void (*onRecieved) (unsigned char dataLength, byte* incomingData, unsigned long callbackID)){
    //Check for valid command ID
    if(commandID & ~(0b1111) > 0){
        return false;
    }

    registeredMessages[commandID].onRecieved = onRecieved;
    registeredMessages[commandID].isRequest = false;
    registeredMessages[commandID].exists = true;

    return true;
}


//Call in the loop() function to execute pending CAN requests and commands
void CANTProtocol::execute(){
    #if DEBUG_CAN
        Serial.println("L");
    #endif
    //Sort all the incoming CAN frames into their respective queues 
    while(frameQueueLength > 0){
        #if DEBUG_CAN
            Serial.println("Q");
            Serial.println(frameQueueFront);
        #endif
        PendingCANFrame* incoming = &pendingFrames[frameQueueFront];
        if(registeredMessages[incoming->dataID].exists){
            if(registeredMessages[incoming->dataID].isRequest){
                //If the length of the queue is 32 frames or greater (max size), drop it. 
                if(requestQueueLength > 31) break;

                int insertLocation = requestQueueFront + requestQueueLength;
                //If we're trying to insert at an index off the end of the array, wraparound to the front
                if(insertLocation > 31) insertLocation = insertLocation - 32;

                //Copy stuff into the queue object
                memcpy(pendingRequests[insertLocation].data, incoming->data, incoming->dataLength);
                pendingRequests[insertLocation].dataLength = incoming->dataLength;
                pendingRequests[insertLocation].callbackID = incoming->callbackID;
                pendingRequests[insertLocation].dataID = incoming->dataID;
                requestQueueLength++;
            }else{
                //If the length of the queue is 32 frames or greater (max size), drop it. 
                if(commandQueueLength > 31) break;

                int insertLocation = commandQueueFront + commandQueueLength;
                //If we're trying to insert at an index off the end of the array, wraparound to the front
                if(insertLocation > 31) insertLocation = insertLocation - 32;

                //Copy stuff into the queue object
                memcpy(pendingCommands[insertLocation].data, incoming->data, incoming->dataLength);
                pendingCommands[insertLocation].dataLength = incoming->dataLength;
                pendingCommands[insertLocation].callbackID = incoming->callbackID;
                pendingCommands[insertLocation].dataID = incoming->dataID;
                commandQueueLength++;
            }
        }
        if(frameQueueFront == 31) frameQueueFront = 0;
        else frameQueueFront++;
        frameQueueLength--;
        
    }


    //Execute up to 2 pending requests
    
    for(int i = 0; i < 2; i++){
        if(requestQueueLength > 0){
            #if DEBUG_CAN
                Serial.println("R");
            #endif
            PendingCANFrame* r = &pendingRequests[requestQueueFront];
            if(requestQueueFront == 31) requestQueueFront = 0;
            else requestQueueFront++;
            requestQueueLength--;

            RegisteredBase request = registeredMessages[r->dataID];
            if(request.exists){
                //User's onRecieved method is expected to call sendRequestResponse() at the end
                request.onRecieved(r->dataLength, r->data, r->callbackID);
            }
             
        }
    }

    //Execute up to 6 pending commands
    for(int i = 0; i < 6; i++){
        if(commandQueueLength > 0){
            #if DEBUG_CAN
                Serial.println("C");
            #endif
            PendingCANFrame* r = &pendingCommands[commandQueueFront];
            if(commandQueueFront == 31) commandQueueFront = 0;
            else commandQueueFront++;
            commandQueueLength--;
            
            
            RegisteredBase command = registeredMessages[r->dataID];
            if(command.exists){
                //The onRecieved method should *not* call sendRequestResponse()
                command.onRecieved(r->dataLength, r->data, r->callbackID);

                //If there is a callback, send ack bit. Otherwise do nothing
                if(r->callbackID > 0){
                    byte outputBuffer[8];
                    outputBuffer[0] = 0x43; //TODO: PROPERLY DEFINE THE ACK BIT
                    byte sendMSG = CAN.sendMsgBuf(r->callbackID, 1, 1, outputBuffer);

                }
            }
        }
    }

}

bool CANTProtocol::end(){
    detachInterrupt(digitalPinToInterrupt(CAN_INTERRUPT_PIN));
    CAN.setMode(MCP_SLEEP);
}

//ISRs have to be static functions. This calls the ISR of a specific instance of the CANT protocol
//This does currently limit the amount of independent CAN controllers a device can have to 1, shouldn't be an issue for 2025-26
static void CANTProtocol::ISRHandler(CANTProtocol* ref){
    ref->InterruptSubroutine();
}

//DON'T CALL THIS YOURSELF. THIS WILL MESS THINGS UP
void CANTProtocol::InterruptSubroutine(){
    
    noInterrupts();
    long unsigned int rxId = 0;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    //Interrupt goes low and stays low until all buffers are empty
    while(CAN.readMsgBuf(&rxId, &len, rxBuf) != CAN_NOMSG){
        #if DEBUG_CAN
         Serial.println("ISR");
        #endif 

        //If the length of the queue is 32 frames or greater (max size), drop it. 
        if(frameQueueLength > 31) break;

        int insertLocation = frameQueueFront + frameQueueLength;
        //If we're trying to insert at an index off the end of the array, wraparound to the front
        if(insertLocation > 31) insertLocation = insertLocation - 32;

        memcpy(&pendingFrames[insertLocation].data, &rxBuf, len);
        pendingFrames[insertLocation].dataLength = len;
        pendingFrames[insertLocation].callbackID = (rxId & 0x000FFFFF);
        pendingFrames[insertLocation].dataID = ((rxId & 0x00F00000) >> 20); 

        frameQueueLength++;
    }
    interrupts();
}