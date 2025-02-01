
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

class ProcedureD{

};

class Creator
{
    private:

        using ProcedureInstantiationFunc = std::function<std::unique_ptr<Procedure>()>;

        // Dear lord this is horrific. Also this can't be an unordered_set because function can't be hashed.
        std::unordered_map<Command, std::vector<ProcedureInstantiationFunc>> procedureCreator;

    public:

        template <typename ProcedureType, typename... Args>
        void bindCommand(Command command, Args... args){

            static_assert(std::is_base_of<Procedure, ProcedureType>(), "ProcedureType must derive from Procedure");

            // This function stores the "template" for how to create this object.
            std::function<std::unique_ptr<Procedure>()> procedureInstantiationFunc = [=](){
                return std::make_unique<ProcedureType>(args...);
            };

            // procedureCreator[command].insert(procedureInstantiationFunc);

            std::cout << "Successfully got past the assert" << std::endl;
        }

        void receiveCommand(Command command){

            

        }


};

int main(){

    Creator creator;

    creator.bindCommand<ProcedureA>(Command::START, "hello?", 3);
    creator.bindCommand<ProcedureB>(Command::START, 26, 87);


}
