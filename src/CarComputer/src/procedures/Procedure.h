/*
 * 
 *
 * Matthew Larson
*/


#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <string>
#include <iostream>

class Procedure{
    public:
        virtual std::string toString() {return "Implement please";}
        // All of these functions can be overridden.
        virtual void init() {;}
        virtual void execute() {;}
        virtual void end() {;}

        // When the condition inside this function is true, the procedure ends.
        virtual bool isFinished() {return false;}
};

#endif