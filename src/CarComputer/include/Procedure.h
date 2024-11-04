/*
 * 
 *
 * Matthew Larson
*/


#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <string>
#include <iostream>


#include "Constants.h"

class Procedure{
    public:
        virtual std::string toString() {return "Insert name of procedure here";}
        // All of these functions can be overridden.
        virtual void init() {;}
        virtual void execute() {;}
        
        // Reset any local variables/values that might need to be reset every time the procedure is initialized and run again.
        virtual void end() {;}

        // When the condition inside this function is true, the procedure ends.
        virtual bool isFinished() {return false;}

        int frequency = BASE_CAR_FREQUENCY;

};

#endif