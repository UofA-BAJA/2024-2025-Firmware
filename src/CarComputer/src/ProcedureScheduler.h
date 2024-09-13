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

        int bindCommand(Procedure* procedure, Command command);

    private:
        std::unordered_set<Procedure*> procedures;
};

#endif