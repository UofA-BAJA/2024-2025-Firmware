#ifndef PROCEDURESCHEDULER_H
#define PROCEDURESCHEDULER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Commands.h"
#include "Procedure.h"
#include "Constants.h"
#include "CarLogger.h"

namespace BajaWildcatRacing
{

    class ProcedureScheduler{

        public:
            ProcedureScheduler();
            void execute();
            void end();

            // https://stackoverflow.com/questions/115703/storing-c-template-function-definitions-in-a-cpp-file
            // :(
            /*
            *  Method:  bindCommand
            *
            *  Purpose: Bind a procedure to a command, so that when the command is received the procedure is run
            *
            *  Pre-Condition:  procedure points to a valid procedure; procedure is not bound to any commands;
            * 
            *  Post-Condition: The procedure is successfully bound to the corresponding command; 
            * 
            * @param procedure -- The procedure to bind
            * @param command -- The command to bind the procedure to
            *
            *  @returns None
            *
            */
            template <typename ProcedureType, typename... Args>
            void bindCommand(std::unordered_set<Command> startCommands, std::unordered_set<Command> endCommands, Args&&... args){

                static_assert(std::is_base_of<Procedure, ProcedureType>(), "ProcedureType must derive from Procedure");

                int uid = currUID;
                currUID++;

                // This function stores the "template" for how to create this object.
                ProcedureInstantiationFunc procedureInstantiationFunc = [&](){
                    return std::make_unique<ProcedureType>(args...);
                };

                procedureTemplates.insert(ProcedureTemplate(uid, procedureInstantiationFunc, startCommands, endCommands));
            }

            void receiveComCommand(Command command);

        private:

            using ProcedureInstantiationFunc = std::function<std::unique_ptr<Procedure>()>;
            

            class ProcedureTemplate{
                public:

                    ProcedureTemplate(int uid,
                                    ProcedureInstantiationFunc instantiationFunc,
                                    std::unordered_set<Command> startCommands,
                                    std::unordered_set<Command> endCommands){
                        this->uid = uid;
                        this->instantiationFunc = instantiationFunc;
                        this->startCommands = startCommands;
                        this->endCommands = endCommands;
                    }

                    int uid;
                    ProcedureInstantiationFunc instantiationFunc;
                    std::unordered_set<Command> startCommands;
                    std::unordered_set<Command> endCommands;

                    bool operator==(const ProcedureTemplate &other) const{ 
                        return (uid == other.uid);
                    }

            };

            // Idk how this syntax works, but I guess you need it to be able to hash the ProcedureTemplates
            struct ProcedureTemplateHasher {
                size_t operator()(const ProcedureTemplate& p) const
                {
                    return std::hash<int>()(p.uid);
                }

            };

            // A set of procedure templates, where the template is identified by its uid
            std::unordered_set<ProcedureTemplate, ProcedureTemplateHasher> procedureTemplates;

            // Maps the procedure template uid to the active procedure
            std::unordered_map<ProcedureTemplate, std::unique_ptr<Procedure>, ProcedureTemplateHasher> activeProcedures;

            int currUID = 0;

    };

}

#endif