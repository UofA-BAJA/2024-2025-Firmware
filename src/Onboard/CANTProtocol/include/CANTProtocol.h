#ifndef CANTPROTOCOL_H
#define CANTPROTOCOL_H

#include <mcp_can.h>
#include <Arduino.h>


class CANTProtocol{
    public:
        /*
            Constructor
            @param cs the pin number of the chip select pin
            @param interrupt the interrupt number. **FOR DEV BOARDS, WRAP PARAMETER IN digitalPinToInterrupt(your_interrupt_pin)**
            @param deviceID the 5 bit ID of the device. 
        */
        CANTProtocol(int cs, int interrupt, unsigned long deviceID) : CAN_CS_PIN(cs), CAN_INTERRUPT_PIN(interrupt), CAN_DEVICE_ID(deviceID), CAN(cs){}
        bool begin();
        bool end();
        

        bool registerRequest(byte commandID, void (*onRecieved) (unsigned char dataLength, byte* incomingData, unsigned long callbackID));
        bool registerCommand(byte commandID, void (*onRecieved) (unsigned char dataLength, byte* incomingData, unsigned long callbackID));
        
        void execute();

        //TODO: move this into a new file
        //Why is it in the header file? https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

        //Call this method with your built data at the end of a CAN *Request* onRecieved method.
        //Do NOT call this method for CAN commands (what would the data be anyway?)
        template <typename T>
        void sendRequestResponse(T data, unsigned long callbackID){
            if(callbackID > 0){
                byte outputBuffer[8];
                //If we need to send multiple frames back
                if(sizeof(T) > 8){
                    // Serial.print("Responding to ");
                    // Serial.println(callbackID, HEX);
                    int numFrames = (sizeof(T) / 8) + 1;
                    byte* ptr = (byte*) &data; //Get a pointer that increments by bytes
                    for(int j = 0; j < numFrames; j++){
                        //(this could be one line but I was having trouble thinking of the right way to do it)
                        //If this is the last frame, memcpy the right amount
                        if(j == numFrames - 1){
                            // Serial.print("Sending end frame of length: ");
                            // Serial.print(sizeof(T) - (j * 8));
                            // Serial.print(" and ID ");
                            // Serial.println(callbackID + j, HEX);
                            memcpy(&outputBuffer, ptr + (j * 8), sizeof(T) - (j * 8));
                            byte sendMSG = CAN.sendMsgBuf(callbackID + j, 1, sizeof(T) - (j * 8), outputBuffer);
                            // if(sendMSG == CAN_GETTXBFTIMEOUT) Serial.println("Get TX Buffer Timeout");
                            // else if(sendMSG == CAN_SENDMSGTIMEOUT) Serial.println("Send Message Timeout");
                            // else if(sendMSG != CAN_OK) Serial.println("Send message failed unknown");
                        }
                        //Otherwise, copy 8 bytes
                        else {
                            // Serial.print("Sending start/mid frame of length: 8 and ID ");
                            // Serial.println(callbackID + j, HEX);
                            memcpy(&outputBuffer, ptr + (j * 8), 8);
                            byte sendMSG = CAN.sendMsgBuf(callbackID + j, 1, 8, outputBuffer);
                            // if(sendMSG == CAN_GETTXBFTIMEOUT) Serial.println("Get TX Buffer Timeout");
                            // else if(sendMSG == CAN_SENDMSGTIMEOUT) Serial.println("Send Message Timeout");
                            // else if(sendMSG != CAN_OK) Serial.println("Send message failed unknown");
                        }
                        //TODO: do something if it fails to send 
                    }
                }
            }
        }

        static void ISRHandler(CANTProtocol* ref);
        void InterruptSubroutine();

        MCP_CAN CAN;

    private:
        const int CAN_CS_PIN;
        const int CAN_INTERRUPT_PIN; 
        const unsigned long CAN_DEVICE_ID;
        

        //Chat-assisted code begins here
        /*
            Basically, in order to have one function to call, but multiple different implementations (one including a template) in the same array,
            you have to have a base struct, and then structs that derive from that base struct.
            call() carries out all the functions, including calling the dataBuilder()/onRecieved() and sending out CAN frames, if nessecary.
            This kinda sucks being in the header file, in the future I might figure out if there's a way to split this off into sub files
            without it breaking the templates (see above: registerRequest())
        */
        struct RegisteredBase {
            bool exists = false;
            void (*onRecieved) (unsigned char dataLength, byte* incomingData, unsigned long callbackID) = 0;
            bool isRequest = false;
        };

        //A list of all the messages we can recieve. Each array index is a datatype as defined in the CANT spec 
        RegisteredBase registeredMessages[16];
        //Chat-assisted code ends here

 
        //This struct holds the data and callback ID for pending requests and commands
        struct PendingCANFrame{
            byte data[8]; //Recieved data
            unsigned char dataLength;
            unsigned long dataID;
            unsigned long callbackID;
        };
        
        /*
            These implement 3 queues for pending CAN stuff.
            The ISR has to be incredibly short and simple, so it copies critical data into pendingFrames. 
            pendingFrames basically acts like an extended recieve buffer.

            In execute(), these pending frames are copied to queues for requests and commands. 
            This allows us granular control over how many requests and commands can be executed per cycle.
            For example, many requests taking a long time to fulfill can't fully overpower critical commands coming in.
            We could have pendingFrames be the only buffer, but if we got, say, 5 requests in a row and then some critical
            command comes in, we'd have to sift through all 5 requests first. With this system, we can only sift through, say,
            2 of those requests before moving on to sift through many commands. 
        */
        volatile PendingCANFrame pendingFrames[32];
        volatile int frameQueueFront = 0;
        volatile int frameQueueLength = 0;

        PendingCANFrame pendingRequests[32];
        int requestQueueFront = 0;
        int requestQueueLength = 0;
        PendingCANFrame pendingCommands[32];
        int commandQueueFront = 0;
        int commandQueueLength = 0;

};






#endif