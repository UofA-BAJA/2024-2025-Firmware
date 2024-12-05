#include "Coms.h"

Coms::Coms(ProcedureScheduler* procedureScheduler){
    this->procedureScheduler = procedureScheduler;

    radioThread = std::thread(&Coms::executeRadio, this);

}


void Coms::execute(float timestamp){

    // Probably necessary to add a mutex here?






}

void Coms::executeRadio(){


    while(true){

        // The car can have 32 streams.
        // Each packet can store 6 data values (Look at data packet struct in header file)
        // ceil(32 / 6) = 6, so there are at most 6 structs we can send in one cycle
        // hence: the array size of 6

        int maxPackets = 6;
        DataPacket packets[maxPackets];

        std::unique_lock<std::mutex> lock(dataStreamMutex);
        
        // Procedure to encode a live data packet
        int currPacket = -1;
        for(int i = 0; i < 32; i++){

            if(i % maxPackets == 0){
                currPacket++;
            }
            
            packets[currPacket].streamMask |= 1 << liveDataStreams[i]->getDataType();
            packets[currPacket].timestamp = currTimestamp;
            packets[currPacket].data[i % currPacket] = liveDataStreams[i]->Dequeue();
        }

        lock.unlock();


        // Now handle the stuff 




    }

}



void Coms::AddNewStream(LiveDataStream* stream){
    liveDataStreams[liveStreamCount] = stream;
    liveStreamCount++;
}
