/*
 *
 *  Class: Coms
 *
 *         Author:  Matthew Larson
 *
 *        Purpose:  Implement the transfer of data, state handling, and radio interfacing
 *        required for sending data between the pit and the car. Due to the fact that the
 *        radio implementations are unique to each radio, this class is not very well
 *        abstracted. I considered splitting the coms class up into a radio class and a coms
 *        class, but I decided it wasn't worth it. If somebody wants to do that one day, 
 *        feel free to.
 *
 *  Inherits From:  None
 *
 *     Interfaces:  None
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:
 *          bool RADIO_ACTIVE
 *
 *+-----------------------------------------------------------------------
 *
 *   Constructors:
 *      Coms(ProcedureScheduler* procedureScheduler) -- initializes fields
 *      and begins the thread that runs the radio hardware 
 *
 *  Class Methods:  [List the names, arguments, and return types of all
 *                   public class methods.]
 *
 *  Inst. Methods:  [List the names, arguments, and return types of all
 *                   public instance methods.]
 *
*/

#include "Coms.h"

#include <bitset>
// To look at the example linux implementation for the nrf24l01, look at the RF24 github:
// https://github.com/nRF24/RF24/tree/master
// Specifically, this file:
// https://github.com/nRF24/RF24/blob/master/examples_linux/gettingstarted.cpp


#define CSN_PIN 1
#define CE_PIN 22

// This radio should only be accessed from the radioThread
// We do not want a mutex to protect it, as it would bee very slow
// and defeat the purpose of multithreading
RF24 radio(CE_PIN, CSN_PIN);

Coms::Coms(ProcedureScheduler* procedureScheduler){
    
    this->procedureScheduler = procedureScheduler;

    radioThread = std::thread(&Coms::executeRadio, this);
}


void Coms::execute(float timestamp){

    std::lock_guard<std::mutex> lock(timestampMutex);
    currTimestamp = timestamp;
}

void Coms::executeRadio(){

    // perform hardware check
    if (!radio.begin()) {
        std::cout << "radio hardware is not responding!!" << std::endl;
        return; // quit now
    }

    // to use different addresses on a pair of radios, we need a variable to
    // uniquely identify which address this radio will use to transmit
    bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
    uint8_t address[2][6] = {"1Node", "2Node"};
    radio.setPayloadSize(32); // float datatype occupies 4 bytes
    radio.setPALevel(RF24_PA_HIGH); // RF24_PA_MAX is default.

    // radio.setRetries(0, 0);

    radio.enableAckPayload();

    radio.openWritingPipe(address[radioNumber]); // always uses pipe 0
    radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

    radio.startListening();


    // If RADIO_ACTIVE is true, this thread will run the entire time the car is on
    while(RADIO_ACTIVE){

        if(currentRadioState == RadioState::RECEIVING){
            radioReceive();
        }
        else if(currentRadioState == RadioState::TRANSMITTING){
            radioTransmit();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void Coms::radioReceive(){

    // Idk if there will be many types of receiving. I guess maybe we might need to send a 
    // stream of data from the pit to the car...?

    // TODO: Packet format from pit still to be determine, for now assume that any packet received means "live data transmit"
    uint8_t pipe;

    int payload[32];

        std::cout << "Receiving mode" <<std::endl;

    while (radio.available(&pipe)) {                     // is there a payload? get the pipe number that received it

        std::cout << "Received payload" <<std::endl;

        uint8_t bytes = radio.getPayloadSize();          // get the size of the payload
        radio.read(&payload, bytes);                     // fetch payload from FIFO
        // cout << "Received " << (unsigned int)bytes;      // print the size of the payload
        // cout << " bytes on pipe " << (unsigned int)pipe; // print the pipe number
        // cout << ": " << packetsReceived << endl;                 // print the payload's value

        int ackPayload[] = {1};

        radio.writeAckPayload(pipe, ackPayload, 1);


        tryUpdateState();
    }
}

void Coms::radioTransmit(){
    if(currentPitCommandState == PitCommandState::LIVE_DATA_TRANSMIT){
        transmitLiveData();
    }
    else if(currentPitCommandState == PitCommandState::DATABASE_TRANSMIT){
        transmitDatabase();
    }
}

void Coms::transmitLiveData(){
        // The car can have 32 streams.
        // Each packet can store 6 data values (Look at data packet struct in header file)
        // ceil(32 / 6) = 6, so there are at most 6 structs we can send in one cycle
        // hence: the array size of 6

        DataPacket packets[maxPackets];

        std::unique_lock<std::mutex> lock(dataStreamMutex);
        
        for(int i = 0; i < maxPackets; i++){
            packets[i].streamMask = 0;
        }

        // Procedure to encode a live data packet
        int currPacket = -1;


        // liveDataStream array MUST be sorted using the data types as the key. This sorting logic is handled when
        // inserting a new stream.

        for(int i = 0; i < liveStreamCount; i++){


            if(i % maxPackets == 0){
                currPacket++;
            }
            

            if(liveDataStreams[i] == nullptr){
                // Something went wrong with assigning the live data stream.
                CarLogger::LogError("Live Data Stream set up incorrectly");
                continue;
            }

            if(!liveDataStreams[i]->dataInQueue()){
                continue;
            }

            // The range of getDataType() should be from 0 to 31. This is described in the packet
            // description of the live data stream packets.
            packets[currPacket].streamMask |= 1 << liveDataStreams[i]->getDataType();
            packets[currPacket].timestamp = currTimestamp;
            packets[currPacket].data[i % maxPackets] = liveDataStreams[i]->dequeue();


            // std::bitset<32> x(packets[currPacket].streamMask);

            // std::cout << x << std::endl;
        }

        lock.unlock();


        // Now we need to send all of the packets that actually have data.
        // Easy way to tell if it has data is if the stream mask != 0

        for(int i = 0; i < maxPackets; i++){

            if(packets[i].streamMask == 0){
                continue;
            }

            bool report = radio.write(&packets[i], sizeof(packets[i]));

            if(report){
                tryUpdateState();       // Check to see if we received a payload from the car, update state if neccesary
            }
            else{
                // Transmission "failed" (it just means there's no acknowledgement)
                // This means that we are out of range for receiving commands from the car,
                // But it still might be possible to transmit to the car.
                // Hence, in here we should just keep transmitting what we have, hoping that the pit is receiving.
            }
        }

}

void Coms::transmitDatabase(){
    // We want to transmit the SQLite database that is stored on the drive of the car
    // I am currently not sure if I should do this by sending the raw data, or
    // somehow encoding the entire database and sending it over. Most likely though,
    // we only want to send one session at a time (probably the current one)
}


void Coms::tryUpdateState(){

    if(currentRadioState == RadioState::RECEIVING){

        currentRadioState = RadioState::TRANSMITTING;

        radio.stopListening();

    }
    else if(currentRadioState == RadioState::TRANSMITTING){

        // If we are transmitting we need to read the acknowledgement packet
        // which stores the command from the pit

        if(radio.isAckPayloadAvailable()){

            int ackData[1];

            radio.read(&ackData, sizeof(ackData));

            std::cout << "Acknowledgement Packet Received" << std::endl;
            currentRadioState = RadioState::RECEIVING;
            radio.startListening();
        }

    }
}


/*
*  Method: addNewLiveDataStream
*
*  Purpose:  Adds a new stream to the list of streams that will be sent over the radio
*
*  Pre-condition: stream is not null
*
*  Post-condition: stream is added to the list of live streams; The array of streams is sorted 
*  using a the stream data type as a key.
*
*  Parameters:
*      LiveDataStream* stream -- a stream to read and send the data over coms 
*
*  Returns:  None
*
*/
void Coms::addNewLiveDataStream(LiveDataStream* stream){

    if(stream == nullptr){
        return;
    }

    // Insertion sort using stream.getDataType() as a key
    DataTypes streamDataType = stream->getDataType();
    
    int i = liveStreamCount;

    while(i > 0 && liveDataStreams[i - 1]->getDataType() > streamDataType){

        // Shift the pointer
        liveDataStreams[i] = liveDataStreams[i - 1];
        i--;
    }

    liveDataStreams[i] = stream;
    liveStreamCount++;
}
