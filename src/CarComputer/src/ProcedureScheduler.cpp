
#include <unordered_set>
#include "ProcedureScheduler.h"
#include "procedures/Procedure.h"
#include "Commands.h"


void ProcedureScheduler::init(){
    // for(auto procedure : procedures){
    //     procedure->init();
    // }
}

void ProcedureScheduler::execute(){

    for(auto keyValuePair : activeProcedures){
        
        auto& procedures = keyValuePair.second;  // Reference to the set of procedures

        for(auto it =  procedures.begin(); it != procedures.end(); ){
            Procedure* procedure = *it;

            if(procedure->isFinished()){
                procedure->end();
                // Remove the procedure from the active procedures list that way it doesn't get run.
                activeProcedures[keyValuePair.first].erase(procedure);
            }
            else{
                procedure->execute();
                ++it;   // Only increment the iterator if we're not erasing anything.
            }

        }
    }
}

void ProcedureScheduler::end(){
    for(auto keyValuePair : activeProcedures){
        for(auto procedure : activeProcedures[keyValuePair.first]){
            procedure->end();
            activeProcedures[keyValuePair.first].erase(procedure);
        }
    }
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
int ProcedureScheduler::bindCommand(Procedure* procedure, Command command){

    totalProcedures[command].insert(procedure);
    // if(totalCommandMap.find(command) == totalCommandMap.end()){
    //     // If the command does not exist, first add the command to the command map

    // }


    // procedures.insert(procedure);

    return EXIT_SUCCESS;
}

/* 
 * If we receive a comm command, we gotta set all the bounded commands as active
 */
int ProcedureScheduler::receiveComCommand(Command command){

    // Iterates through all the procedures bounded to command
    for(auto procedure : totalProcedures[command]){

        // If the procedure is already active, then obviously we don't want to activate it again, right...?
        if(activeProcedures[command].find(procedure) == activeProcedures[command].end()){
            procedure->init();
            // Technically insert uses the find() function already, so there's the possiblity for optimization.
            activeProcedures[command].insert(procedure);
        }
    }

    std::cout << "Command: " << command << " received" << std::endl;

    return EXIT_SUCCESS;
}