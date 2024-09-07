/* 
 * DO NOT TOUCH THIS FILE. 
 * INITIAL SETUP FOR CAR
 * 
 * 
 * Author: Matthew Larson
 */

// I'm not sure why I need this preprocessor, but this works...
// https://stackoverflow.com/questions/68742519/why-cant-i-use-the-nanosleep-function-even-when-time-h-is-includedhttps://stackoverflow.com/questions/68742519/why-cant-i-use-the-nanosleep-function-even-when-time-h-is-included
#define _POSIX_C_SOURCE 199309L 

#include <iostream>
#include <time.h>
#include "car.h"


int main(){
    // TODO: Please test on raspberry pi

    Car car = Car();

    // ! WARNNING: not tested on raspberry pi. 
    // ! Does not work with frequency 1 for whatever reason...
    int frequency = 2;   // CAN can go up to 1 Mhz or 1000000 hz

    float cycleTime = 1.0 / frequency;  // Length of time to sleep
    int cycleTimens = (int)(cycleTime * 1000000000L);

    int i = 0;

    while(1){
        // req defines a time value required by nanosleep 
        struct timespec req = {0};
        req.tv_sec = 0;
        req.tv_nsec = cycleTimens;
        nanosleep(&req, (struct timespec *)NULL);

        car.execute();
    }


    return 0;
}