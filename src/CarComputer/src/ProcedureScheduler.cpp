
#include <unordered_set>
#include "ProcedureScheduler.h"
#include "procedures/Procedure.h"
#include "Commands.h"


void ProcedureScheduler::init(){
    for(auto procedure : procedures){
        procedure->init();
    }
}

void ProcedureScheduler::execute(){
    for(auto procedure : procedures){

        if(procedure->isFinished()){
            procedure->end();

            // Remove the procedure from theh procedures list that way it doesn't get run.
            procedures.erase(procedure);
            continue;


            // THERE"S A MEMORY LEAK HERE
        }

        procedure->execute();

    }
}

void ProcedureScheduler::end(){

    for(auto procedure : procedures){
        procedure->end();
    }
    // for(int i = 0; i < numProcedures; i++){
    //     procedures[i]->end();
    // }
}


/* 
 * Adds a procedure to the scheduler, binding it to the clock, 
 * initialization, and termination functions.
 * 
 * Parameter: 
 *      procedure -- the procedure to bind to the scheduler
 * 
 * Returns: EXIT_SUCCESS for successful binding and
 * EXIT_FAILURE for unsucessful binding. 
 */
int ProcedureScheduler::bindCommand(Procedure* procedure, Command Command){

    // if(numProcedures >= MAX_PROCEDURES){
    //     std::cout << "Number of commands is at its max. ";
    //     std::cout << procedure->toString() << " will not be included in the car.\n";
    //     return EXIT_FAILURE;
    // }

    procedures.insert(procedure);
    // numProcedures++;

    return EXIT_SUCCESS;
}