
#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <vector>

enum Command{
    START,
    END,
    EMERGENCY
};

class Procedure{
    public:
        virtual void TestFunc()
        {
            std::cout << "Base class function called" << std::endl;
        }
        virtual void End()
        {
            std::cout << "End function called" << std::endl;
        }
        virtual void Init()
        {
            std::cout << "Init function called" << std::endl;
        }

        ~Procedure(){
            std::cout << "Object destroyed :(" << std::endl;
        }
};

class ProcedureA : public Procedure{
    public:
        ProcedureA(const char* str, int val){
            this->val = val;
            std::cout << "str: " << str << " val: " << val << std::endl;
        }

        void TestFunc() override{
            std::cout << "Child class function called. val: " << val << std::endl;
        }

    private:

        int val;

};
class ProcedureB : public Procedure{
    public:
        ProcedureB(int val1, int val2){
            this->val1 = val1;
            std::cout << "val1: " << val1 << " val2: " << val2 << std::endl;
        }

        void TestFunc() override{
            std::cout << "Child class function called. val: " << val1 << std::endl;
        }
        
    private:

        int val1;
};
class ProcedureC : public Procedure{

};

class ProcedureD : public Procedure{

};

class Creator
{
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

    public:

        template <typename ProcedureType, typename... Args>
        void bindCommand(std::unordered_set<Command> startCommands, std::unordered_set<Command> endCommands, Args... args){

            static_assert(std::is_base_of<Procedure, ProcedureType>(), "ProcedureType must derive from Procedure");

            int uid = currUID;
            currUID++;

            // This function stores the "template" for how to create this object.
            ProcedureInstantiationFunc procedureInstantiationFunc = [=](){
                return std::make_unique<ProcedureType>(args...);
            };

            procedureTemplates.insert(ProcedureTemplate(uid, procedureInstantiationFunc, startCommands, endCommands));

            // std::cout << "Successfully got past the assert" << std::endl;
        }

        void receiveCommand(Command command){

            for(auto it = procedureTemplates.begin(); it != procedureTemplates.end(); ){

                ProcedureTemplate procedureTemplate = *it;

                // Check to see if the command exists in the template's end commands. If it does, then check
                // if that procedure associated with the command is active and end it.
                if(procedureTemplate.endCommands.find(command) != procedureTemplate.endCommands.end()){
                    
                    if(activeProcedures.find(procedureTemplate) != activeProcedures.end()){

                        activeProcedures[procedureTemplate]->End();
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
                        activeProcedures[procedureTemplate]->Init();

                    }
                }
                ++it;

            }
            

        }


};

int main(){

    Creator creator;

    creator.bindCommand<ProcedureA>(
        std::unordered_set<Command>({Command::START}),
        std::unordered_set<Command>({Command::END}),
        "hello?", 3);

    creator.bindCommand<ProcedureB>(
        std::unordered_set<Command>({Command::END}),
        std::unordered_set<Command>({Command::END}),
        44, 3);
        
    // creator.bindCommand<ProcedureD>(
    //     std::unordered_set<Command>({}),
    //     std::unordered_set<Command>({Command::EMERGENCY, Command::END})
    //     );

    creator.receiveCommand(Command::START);
    std::cout << std::endl;
    creator.receiveCommand(Command::END);
    std::cout << std::endl;
    creator.receiveCommand(Command::START);
    std::cout << std::endl;
    creator.receiveCommand(Command::END);
    std::cout << std::endl;



}
