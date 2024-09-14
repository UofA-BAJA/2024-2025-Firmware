#ifndef PROCEDURESCHEDULER_H
#define PROCEDURESCHEDULER_H

#include <unordered_set>
#include <unordered_map>
#include "Commands.h"
#include "procedures/Procedure.h"

class ProcedureScheduler{
    public:
        void init();
        void execute();
        void end();

        int bindCommand(Procedure* procedure, Command command);
        int receiveComCommand(Command command);

    private:
        std::unordered_map<Command, std::unordered_set<Procedure*>> totalProcedures;
        std::unordered_map<Command, std::unordered_set<Procedure*>> activeProcedures;

};

#endif