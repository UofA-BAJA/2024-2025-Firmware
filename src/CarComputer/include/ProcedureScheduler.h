#ifndef PROCEDURESCHEDULER_H
#define PROCEDURESCHEDULER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
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

            void bindCommand(std::unique_ptr<Procedure> procedure, Command command);
            void receiveComCommand(Command command);

            template <typename Procedure, typename... Args>
            void bindCommand(Command command, Args... args);

        private:
            std::unordered_map<Command, std::unordered_set<Procedure*>> totalProcedures;
            std::unordered_map<Command, std::unordered_set<Procedure*>> activeProcedures;

    };

}

#endif