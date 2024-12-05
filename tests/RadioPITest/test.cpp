/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use `ctrl+c` to quit at any time.
 */
#include <ctime>       // time()
#include <iostream>    // cin, cout, endl
#include <string>      // string, getline()
#include <time.h>      // CLOCK_MONOTONIC_RAW, timespec, clock_gettime()
#include <RF24/RF24.h> // RF24, RF24_PA_LOW, delay()

using namespace std;

/****************** Linux ***********************/
// Radio CE Pin, CSN Pin, SPI Speed
// CE Pin uses GPIO number with BCM and SPIDEV drivers, other platforms use their own pin numbering
// CS Pin addresses the SPI bus number at /dev/spidev<a>.<b>
// ie: RF24 radio(<ce_pin>, <a>*10+<b>); spidev1.0 is 10, spidev1.1 is 11 etc..
#define CSN_PIN 1
#define CE_PIN 22
// Generic:
RF24 radio(CE_PIN, CSN_PIN);
/****************** Linux (BBB,x86,etc) ***********************/
// See http://nRF24.github.io/RF24/pages.html for more information on usage
// See https://github.com/eclipse/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
int payload[8];

int packetsReceived = 0;

uint64_t failures = 0;


bool switchRoles = false;


// void setRole(); // prototype to set the node's role
// void master();  // prototype of the TX node's behavior
// void slave();   // prototype of the RX node's behavior


void execute();
void receive();
void transmit();

// custom defined timer for evaluating transmission time in microseconds
struct timespec startTimer, endTimer;
uint32_t getMicros(); // prototype to get elapsed time in microseconds




enum RadioState{
    TRANSMITTING,
    RECEIVING
};

RadioState currentState = RadioState::RECEIVING;

struct DataPacket{
    int streamMask;
    float timestamp;
    float data[6];
};



int main(int argc, char** argv)
{

    cout << sizeof(DataPacket) << endl;

    // perform hardware check
    if (!radio.begin()) {
        cout << "radio hardware is not responding!!" << endl;
        return 0; // quit now
    }

    // to use different addresses on a pair of radios, we need a variable to
    // uniquely identify which address this radio will use to transmit
    bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

    // print example's name
    cout << argv[0] << endl;

    // Let these addresses be used for the pair
    uint8_t address[2][6] = {"1Node", "2Node"};
    // It is very helpful to think of an address as a path instead of as
    // an identifying device destination


    // radio.enableAckPayload();

    // save on transmission time by setting the radio to only transmit the
    // number of bytes we need to transmit a float
    // radio.setPayloadSize(sizeof(DataPacket)); // float datatype occupies 4 bytes
    radio.setPayloadSize(32); // float datatype occupies 4 bytes

    // Set the PA Level low to try preventing power supply related problems
    // because these examples are likely run with nodes in close proximity to
    // each other.
    radio.setPALevel(RF24_PA_HIGH); // RF24_PA_MAX is default.

    radio.setDataRate(RF24_2MBPS);
    radio.setAutoAck(false);
    // set the TX address of the RX node into the TX pipe
    radio.openWritingPipe(address[radioNumber]); // always uses pipe 0

    radio.setRetries(0, 0);
    // set the RX address of the TX node into a RX pipe
    radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

    // For debugging info
    // radio.printDetails();       // (smaller) function that prints raw register values
    // radio.printPrettyDetails(); // (larger) function that prints human readable data

    // ready to execute program now


    // Execution clock?

    while(true){
        execute();
        // delay(1);
    }

    return 0;
}




void execute(){
    // Execute the "state machine" here. Not really a state machine but whatever
    if(currentState == RadioState::RECEIVING){
        radio.startListening(); // put radio in RX mode
        receive();
    }
    else if(currentState == RadioState::TRANSMITTING){
        radio.stopListening(); // put radio in TX mode
        transmit();
    }
}



void receive(){

    cout << "Attempting to receive" << endl;
    // If we want to switch roles on the computer side, we need to send an acknowledgement payload
    // to indicate a request.
    // We will then switch 
    if(switchRoles){
        int ackData[1] = {1};
        radio.writeAckPayload(1, &ackData, sizeof(ackData));


        currentState = RadioState::TRANSMITTING;

        switchRoles = false;
    }

    uint8_t pipe;
    while (radio.available(&pipe)) {                        // is there a payload? get the pipe number that received it

        packetsReceived++;

        uint8_t bytes = radio.getPayloadSize();          // get the size of the payload
        radio.read(&payload, bytes);                     // fetch payload from FIFO
        cout << "Received " << (unsigned int)bytes;      // print the size of the payload
        cout << " bytes on pipe " << (unsigned int)pipe; // print the pipe number
        cout << ": " << packetsReceived << endl;                 // print the payload's value
    }






}




void transmit(){


    clock_gettime(CLOCK_MONOTONIC_RAW, &startTimer);    // start the timer
    bool report = radio.write(&payload, sizeof(float)); // transmit & save the report
    uint32_t timerElapsed = getMicros();                // end the timer

    if (report) {
        // payload was delivered
        cout << "Transmission successful! Time to transmit = ";
        cout << timerElapsed;                     // print the timer result
        cout << " us. Sent: " << payload << endl; // print payload sent


        // If there is an acknowledgement payload, that means the pit wants to take control.
        // That being said, we might eventually want to read the payload data and do something with it
        if(radio.isAckPayloadAvailable()){
            currentState = RadioState::RECEIVING;
        }
        else{
            // No payload, continue as normal
        }

    }
    else {
        // payload was not delivered
        cout << "Transmission failed or timed out" << endl;
        failures++;
        }

    // to make this example readable in the terminal
    delay(10); // slow transmissions down by 10 milliseconds


}


// /**
//  * set this node's role from stdin stream.
//  * this only considers the first char as input.
//  */
// void setRole()
// {
//     string input = "";
//     while (!input.length()) {
//         cout << "*** PRESS 'T' to begin transmitting to the other node\n";
//         cout << "*** PRESS 'R' to begin receiving from the other node\n";
//         cout << "*** PRESS 'Q' to exit" << endl;
//         getline(cin, input);
//         if (input.length() >= 1) {
//             if (input[0] == 'T' || input[0] == 't')
//                 master();
//             else if (input[0] == 'R' || input[0] == 'r')
//                 slave();
//             else if (input[0] == 'Q' || input[0] == 'q')
//                 break;
//             else
//                 cout << input[0] << " is an invalid input. Please try again." << endl;
//         }
//         input = ""; // stay in the while loop
//     }               // while
// } // setRole()








// void master()
// {
//     radio.stopListening(); // put radio in TX mode

//     unsigned int failure = 0; // keep track of failures
//     while (failure < 6) {
//         clock_gettime(CLOCK_MONOTONIC_RAW, &startTimer);    // start the timer
//         bool report = radio.write(&payload, sizeof(float)); // transmit & save the report
//         uint32_t timerElapsed = getMicros();                // end the timer

//         if (report) {
//             // payload was delivered
//             cout << "Transmission successful! Time to transmit = ";
//             cout << timerElapsed;                     // print the timer result
//             cout << " us. Sent: " << payload << endl; // print payload sent
//             payload += 0.01;                          // increment float payload
//         }
//         else {
//             // payload was not delivered
//             cout << "Transmission failed or timed out" << endl;
//          }

//         // to make this example readable in the terminal
//         delay(10); // slow transmissions down by 1 second
//     }
//     cout << failure << " failures detected. Leaving TX role." << endl;
// }

// /**
//  * make this node act as the receiver
//  */
// void slave()
// {

//     radio.startListening(); // put radio in RX mode

//     time_t startTimer = time(nullptr);       // start a timer
//     while (time(nullptr) - startTimer < 6) { // use 6 second timeout
//         uint8_t pipe;
//         if (radio.available(&pipe)) {                        // is there a payload? get the pipe number that received it
//             uint8_t bytes = radio.getPayloadSize();          // get the size of the payload
//             radio.read(&payload, bytes);                     // fetch payload from FIFO
//             cout << "Received " << (unsigned int)bytes;      // print the size of the payload
//             cout << " bytes on pipe " << (unsigned int)pipe; // print the pipe number
//             cout << ": " << payload << endl;                 // print the payload's value
//             startTimer = time(nullptr);                      // reset timer
//         }
//     }
//     cout << "Nothing received in 6 seconds. Leaving RX role." << endl;
//     radio.stopListening();
// }

/**
 * Calculate the elapsed time in microseconds
 */
uint32_t getMicros()
{
    // this function assumes that the timer was started using
    // `clock_gettime(CLOCK_MONOTONIC_RAW, &startTimer);`

    clock_gettime(CLOCK_MONOTONIC_RAW, &endTimer);
    uint32_t seconds = endTimer.tv_sec - startTimer.tv_sec;
    uint32_t useconds = (endTimer.tv_nsec - startTimer.tv_nsec) / 1000;

    return ((seconds)*1000 + useconds) + 0.5;
}
