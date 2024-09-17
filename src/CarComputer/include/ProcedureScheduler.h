#ifndef PROCEDURESCHEDULER_H
#define PROCEDURESCHEDULER_H

#include <unordered_set>
#include <unordered_map>
#include "Commands.h"
#include "Procedure.h"

class ProcedureScheduler{
    public:
        ProcedureScheduler();
        void execute();
        void end();

        void bindCommand(Procedure* procedure, Command command);
        void receiveComCommand(Command command);

    private:
        std::unordered_map<Command, std::unordered_set<Procedure*>> totalProcedures;
        std::unordered_map<Command, std::unordered_set<Procedure*>> activeProcedures;

};

#endif