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

        for(auto it = activeProcedures.begin(); it != activeProcedures.end(); ){

            ProcedureTemplate procedureTemplate = it->first;

            if(BASE_CAR_FREQUENCY % activeProcedures[procedureTemplate]->frequency != 0){
                std::string errorMsg = activeProcedures[procedureTemplate]->toString() + " frequency not a divisor of 360";
                CarLogger::LogError(errorMsg.c_str());
                ++it;
                continue;
            }

            if(cycleCount % (BASE_CAR_FREQUENCY / activeProcedures[procedureTemplate]->frequency) != 0) {
                ++it;
                continue;
            }

            if(activeProcedures[procedureTemplate]->isFinished()){

                activeProcedures[procedureTemplate]->end();
                activeProcedures.erase(procedureTemplate);
            }
            else{
                activeProcedures[procedureTemplate]->execute();
                ++it;   // Only increment the iterator if we're not erasing anything.
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
        // for(auto keyValuePair : activeProcedures){
        //     for(Procedure* procedure : activeProcedures[keyValuePair.first]){
        //         procedure->end();
        //         activeProcedures[keyValuePair.first].erase(procedure);
        //     }
        // }
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
    * 
    *  @param command -- The command sent from the pit computer to the Raspberry PI
    *
    *  @returns None
    */
    void ProcedureScheduler::receiveComCommand(Command command){

        for(auto it = procedureTemplates.begin(); it != procedureTemplates.end(); ){

            ProcedureTemplate procedureTemplate = *it;

            // Check to see if the command exists in the template's end commands. If it does, then check
            // if that procedure associated with the command is active and end it.
            if(procedureTemplate.endCommands.find(command) != procedureTemplate.endCommands.end()){
                
                if(activeProcedures.find(procedureTemplate) != activeProcedures.end()){

                    activeProcedures[procedureTemplate]->end();
                    activeProcedures.erase(procedureTemplate);
                    continue;
                }
            }

            // Now do the same thing as above, but for the start commands.
            if(procedureTemplate.startCommands.find(command) != procedureTemplate.startCommands.end()){
                
                // Procedures that are already active will just keep running.
                if(activeProcedures.find(procedureTemplate) != activeProcedures.end()){
                    // Do nothing
                }
                else{
                    // Otherwise, create the new procedure and make it active
                    activeProcedures[procedureTemplate] = procedureTemplate.instantiationFunc();
                    activeProcedures[procedureTemplate]->init();

                }
            }
            ++it;

        }

        std::cout << "Command: " << command << " received" << std::endl;
    }

}
