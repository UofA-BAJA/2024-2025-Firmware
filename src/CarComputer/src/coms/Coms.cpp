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
 *  Class Methods:  None
 *
 *  Inst. Methods:  [List the names, arguments, and return types of all
 *                   public instance methods.]
 *
*/

#include "Coms.h"

// To look at the example linux implementation for the nrf24l01, look at the RF24 github:
// https://github.com/nRF24/RF24/tree/master
// Specifically, this file:
// https://github.com/nRF24/RF24/blob/master/examples_linux/gettingstarted.cpp


#define CSN_PIN 1
#define CE_PIN 22

namespace BajaWildcatRacing
{

    Coms::Coms(ProcedureScheduler& procedureScheduler)
    : radio(CE_PIN, CSN_PIN)
    , procedureScheduler(procedureScheduler)
    {
        
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
        bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
        uint8_t address[2][6] = {"1Node", "2Node"};
        radio.setPayloadSize(32); // float datatype occupies 4 bytes
        radio.setPALevel(RF24_PA_HIGH); // RF24_PA_MAX is default.

        // radio.setRetries(0, 0);
        radio.enableDynamicPayloads();
        radio.enableAckPayload();

        radio.openWritingPipe(address[radioNumber]); // always uses pipe 0
        radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

        radio.stopListening();


        int waitTimems = (1.0 / RADIO_CLOCK_FREQUENCY) * 1000;
        // If RADIO_ACTIVE is true, this thread will run the entire time the car is on
        while(RADIO_ACTIVE){

            radioTransmit();


            std::this_thread::sleep_for(std::chrono::milliseconds(waitTimems));
        }
    }


    void Coms::radioTransmit(){
        // if(currentPitCommandState == PitCommandState::IDLE){
        //     idle();
        // }
        // else if(currentPitCommandState == PitCommandState::LIVE_DATA_TRANSMIT){
        //     transmitLiveData();
        // }
        // else if(currentPitCommandState == PitCommandState::DATABASE_TRANSMIT){
        //     transmitDatabase();
        // }

        transmitLiveData();

        // Received command from pit!
        if(radio.isAckPayloadAvailable()){


            int ackData;

            radio.read(&ackData, 1);

            // if(ackData == Command::START_LOG){
            //     // procedureScheduler.receiveComCommand(Command::START_LOG);

            //     currentPitCommandState = PitCommandState::LIVE_DATA_TRANSMIT;
            // }
            // else if(ackData == Command::END_LOG){
            //     currentPitCommandState = PitCommandState::IDLE;
            // }

            std::lock_guard<std::mutex> lock(procedureSchedulerMutex);

            procedureScheduler.receiveComCommand((Command)ackData);

            // std::bitset<32> x(ackData);

            std::cout << ackData << std::endl;
        }

    }

    int idlePacket[] = {0, 0, 0, 0, 0, 0, 0, 0};

    void Coms::transmitLiveData(){
            // The car can have 32 streams.
            // Each packet can store 6 data values (Look at data packet struct in header file)
            // ceil(32 / 6) = 6, so there are at most 6 structs we can send in one cycle
            // hence: the array size of 6

            DataPacket packets[maxPackets] = {};

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


                liveDataStreams[i]->clearAllData();
                // std::bitset<32> x(packets[currPacket].streamMask);

                // std::cout << x << std::endl;
            }


            lock.unlock();


            // Now we need to send all of the packets that actually have data.
            // Easy way to tell if it has data is if the stream mask != 0

            int packetsSent = 0;
            for(int i = 0; i < maxPackets; i++){

                if(packets[i].streamMask == 0){
                    continue;
                }

                bool report = radio.write(&packets[i], sizeof(packets[i]));
                packetsSent++;

                if(report){

                }
                else{
                    // Transmission "failed" (it just means there's no acknowledgement)
                    // This means that we are out of range for receiving commands from the car,
                    // But it still might be possible to transmit to the car.
                    // Hence, in here we should just keep transmitting what we have, hoping that the pit is receiving.
                }
            }

            if(packetsSent == 0){
                bool report = radio.write(idlePacket, sizeof(idlePacket));
            }

    }

    void Coms::transmitDatabase(){
        // We want to transmit the SQLite database that is stored on the drive of the car
        // I am currently not sure if I should do this by sending the raw data, or
        // somehow encoding the entire database and sending it over. Most likely though,
        // we only want to send one session at a time (probably the current one)
    }


    void Coms::sendData(DataTypes dataType, float data){

        // If the stream of type dataType doesn't exist, create it and add it to the list of streams.
        if(liveDataStreamMap.find(dataType) == liveDataStreamMap.end()){
            std::shared_ptr<LiveDataStream> newStream = std::make_shared<LiveDataStream>(dataType);
            liveDataStreamMap[dataType] = newStream;

            addNewLiveDataStream(newStream);

        }

        liveDataStreamMap[dataType]->enqueue(data);

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
    void Coms::addNewLiveDataStream(std::shared_ptr<LiveDataStream> stream){

        if(stream == nullptr){
            return;
        }

        // Insertion sort using stream.getDataType() as a key
        DataTypes streamDataType = stream->getDataType();

        if(streamDataType >= MAX_LIVE_DATA_STREAMS){

            std::string errorStr("Not allowed to have more than " + std::to_string(MAX_LIVE_DATA_STREAMS) + " live data streams!");

            CarLogger::LogError(errorStr.c_str());
            return;
        }
        
        int i = liveStreamCount;

        while(i > 0 && liveDataStreams[i - 1]->getDataType() > streamDataType){

            // Shift the pointer
            liveDataStreams[i] = std::move(liveDataStreams[i - 1]);
            i--;
        }

        liveDataStreams[i] = std::move(stream);
        liveStreamCount++;

    }


}
