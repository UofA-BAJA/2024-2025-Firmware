/* 
 * Represents the car as a whole, controlling the procedure scheduler, car container, and coms.
 * PLEASE DO NOT EDIT THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING!
 * 
 * 
 * Author: Matthew Larson
 */

// I'm not sure why I need this preprocessor, but this works...
// https://stackoverflow.com/questions/68742519/why-cant-i-use-the-nanosleep-function-even-when-time-h-is-includedhttps://stackoverflow.com/questions/68742519/why-cant-i-use-the-nanosleep-function-even-when-time-h-is-included
#define _POSIX_C_SOURCE 199309L

#include "Car.h"

ProcedureScheduler procedureScheduler;
CANDispatcher* canDispatcher;

Car::Car() {
    // Init behavior that needs to be called before the subsystems start running.
    init();

    const char* can_interface = "can0";
    canDispatcher = new CANDispatcher(can_interface);
    CarContainer carContainer = CarContainer(procedureScheduler, canDispatcher);
    procedureScheduler.receiveComCommand(Command::START_LOG);

    execute();
}


Car::~Car(){
    // Later on we'll probably call this function as a command from the pit. Actually, 
    // the car should theoretically never be destroyed unless the program quits, which
    // should never happen while the car is running. 
    end();
}

void Car::execute(){
    // ! WARNNING: not tested on raspberry pi. 
    // ! Does not work with frequency 1 for whatever reason...
    int frequency = 400;   // CAN can go up to 1 Mhz or 1000000 hz

    float cycleTime = 1.0 / frequency;  // Length of time to sleep
    int cycleTimens = (int)(cycleTime * 1000000000L);


    // I'm not exactly sure if this is exactly what we want, but it "should" be good enough for now

    while(1){
        // req defines a time value required by nanosleep 
        struct timespec req = {0};
        req.tv_sec = 0;
        req.tv_nsec = cycleTimens;
        nanosleep(&req, (struct timespec *)NULL);

        procedureScheduler.execute();
    }

    // printing for other subsystems only works if you print something to the terminal...?
    std::cout << "";
}

void Car::init(){
    std::cout << "Car Sucessfully Initialized\n";
}

void Car::end(){
    procedureScheduler.end();
    std::cout << "Car sucessfully destroyed\n";
}
