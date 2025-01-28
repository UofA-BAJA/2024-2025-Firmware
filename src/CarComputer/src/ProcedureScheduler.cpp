/*
 *
 *  Class: ProcedureScheduler
 *
 *         Author:  Matthew Larson
 *
 *        Purpose:  Handles the initialization, execution, and ending of procedures that
 *        are bound to commands.
 *
 *  Inherits From:  None
 *
 *     Interfaces:  None
 *
 *+-----------------------------------------------------------------------
 *
 *      Constants:  None
 *
 *+-----------------------------------------------------------------------
 *
 *   Constructors:  ProcedureScheduler() -- does nothing right now
 *
 *  Class Methods:  [List the names, arguments, and return types of all
 *                   public class methods.]
 *
 * Inst. Methods:  [List the names, arguments, and return types of all
 *                   public instance methods.]
 *
*/


#include "ProcedureScheduler.h"

namespace BajaWildcatRacing
{

    ProcedureScheduler::ProcedureScheduler(){


    }

    /*
    *  Method:  execute
    *
    *  Purpose: Run the execution of all active procedures and check if the procedure should
    *           terminate. If it should, terminate the procedure.
    *
    *  Pre-Condition: None
    * 
    *  Post-Condition: All active procedures are executed; If the procedure should be terminated, 
    *                  it is ended; The procedures are run at their selected frequency
    * 
    *  Parameters: None
    *
    *  Returns: None
    */

    ulong cycleCount = 0;

    void ProcedureScheduler::execute(){

        for(auto keyValuePair : activeProcedures){
            
            auto& procedures = keyValuePair.second;  // Reference to the set of procedures


            // Use of an iterator is required, as we are removing elements from the set we are iterating over.
            for(auto it =  procedures.begin(); it != procedures.end(); ){

                Procedure* procedure = *it;

                if(360 % procedure->frequency != 0){
                    std::string errorMsg = procedure->toString() + " frequency not a divisor of 360";
                    CarLogger::LogError(errorMsg.c_str());
                    ++it;
                    continue;
                }

                if(cycleCount % (BASE_CAR_FREQUENCY / procedure->frequency) != 0) {
                    ++it;
                    continue;
                }

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

        cycleCount++;
    }

    /*
    *  Method:  end
    *
    *  Purpose: Halts the execution of actively running procedures and ends it.
    *
    *  Pre-Condition: None
    * 
    *  Post-Condition: All active procedures are ended
    * 
    *  Parameters: None
    *
    *  Returns: None
    */
    void ProcedureScheduler::end(){
        // ! We're going to get a bug here! We need to do this the way it's done in the 
        // ! execute method above. (The bug will be a )
        for(auto keyValuePair : activeProcedures){
            for(auto procedure : activeProcedures[keyValuePair.first]){
                procedure->end();
                activeProcedures[keyValuePair.first].erase(procedure);
            }
        }
    }


    /*
    *  Method:  bindCommand
    *
    *  Purpose: Bind a procedure to a command, so that when the command is received the procedure is run
    *
    *  Pre-Condition:  procedure points to a valid procedure; procedure is not bound to any commands;
    * 
    *  Post-Condition: The procedure is successfully bound to the corresponding command; 
    * 
    *  Parameters:
    *          procedure -- The procedure to bind
    *
    *          command -- The command to bind the procedure to
    *
    *  Returns: None
    *
    */
    void ProcedureScheduler::bindCommand(Procedure* procedure, Command command){
        totalProcedures[command].insert(procedure);
    }

    /*
    *  Method:  receiveComCommand
    *
    *  Purpose: Activate all the procedures bound to the command sent from the pit computer
    *
    *  Pre-Condition:  command is a valid command recognized by the Raspberry PI;
    * 
    *  Post-Condition: All procedures bound to command are activated;
    * 
    *  Parameters:
    *          command -- The command sent from the pit computer to the Raspberry PI
    *
    *  Returns: None
    */
    void ProcedureScheduler::receiveComCommand(Command command){

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
    }

}
