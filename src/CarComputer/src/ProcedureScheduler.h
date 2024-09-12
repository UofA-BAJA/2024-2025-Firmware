#ifndef PROCEDURESCHEDULER_H
#define PROCEDURESCHEDULER_H

#include <unordered_set>
#include "Commands.h"
#include "procedures/Procedure.h"

class ProcedureScheduler{
    public:
        void init();
        void execute();
        void end();

        int bindCommand(Procedure* procedure, Command);

    private:
        // static const int MAX_PROCEDURES = 256;
        // int numProcedures = 0;
        std::unordered_set<Procedure*> procedures;   // No way we have more than 256 commands.
};

#endif