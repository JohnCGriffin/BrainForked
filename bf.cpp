
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <map>
#include <cstring>

#include "bf.hpp"

namespace bf {


    static Instructions load_instructions(std::istream& is)
    {
        auto instructions = read_instructions(is);

        instructions = optimizations(instructions);


        // WHILE LOOP PATCHING MUST BE LAST
        // so that the mutual begin/end while
        // loop offsets are correct.
    
        std::vector<int> while_stack;
        for(size_t i=0; i<instructions.size(); i++){
        
            if(instructions.at(i).action == FALSEJUMP){
                while_stack.push_back(i);
            }
        
            if(instructions.at(i).action == TRUEJUMP){
                int start = while_stack.back();
                instructions.at(start).val = (i - start);
                instructions.at(i).val = (start - i);
                while_stack.pop_back();
            }
        }

        instructions.push_back({ TERMINATE });

        return instructions;
    }

    
    static void execute(Instructions&  _instructions)
    {
        std::map<Action,void*> addrs;

        // Note that initialization in modern manner chokes clang/g++ 
        // when in c++14/17.  Direct assignment one by one seems to work.
        addrs[ADVANCE]     = &&_ADVANCE;
        addrs[VALUE]       = &&_VALUE;
        addrs[MOVE]        = &&_MOVE;
        addrs[ADD]         = &&_ADD;
        addrs[INCR]        = &&_INCR;
        addrs[TRUEJUMP]    = &&_TRUEJUMP;
        addrs[FALSEJUMP]   = &&_FALSEJUMP;
        addrs[ZERO]        = &&_ZERO;
	addrs[M1]          = &&_M1;
        addrs[MOVEWHILE]   = &&_MOVEWHILE;
        addrs[MOVEINCR]    = &&_MOVEINCR;
        addrs[OFFINCR]     = &&_OFFINCR;
        addrs[WHILEIM2]    = &&_WHILEIM2;
        addrs[WHILEIM3]    = &&_WHILEIM3;
        addrs[PRINT]       = &&_PRINT;
        addrs[READ]        = &&_READ;
        addrs[TERMINATE]   = &&_TERMINATE;

	for(auto a : ENUMERATED_ACTIONS){
            if(!addrs[a]){
		throw std::logic_error(to_string(a) + " not in address table");
            }
        }

        for(auto& instr : _instructions){
            instr.jump = addrs.at(instr.action);
        }

        std::vector<char> characters(30000);
        char* ptr = characters.data();
        auto IP = _instructions.data();

        // initial JUMP.  The remainder happen via the LOOP macro.
        goto *(IP->jump);

#define LOOP() IP++; goto *(IP->jump)//*jumps[IP->action]

    _TRUEJUMP:
        if(*ptr){
            IP += IP->val;
        }
        LOOP();
        
    _FALSEJUMP:     
        if(!(*ptr)){
            IP += IP->val;
        }
        LOOP();

    _WHILEIM3:
        if(ptr[0]){
            const auto a = IP[-6].val;
            const auto b = IP[-5].val;
            const auto c = IP[-4].val;
            const auto d = IP[-3].val;
            const auto e = IP[-2].val;
            const auto f = IP[-1].val;
            do {
		ptr[0] += a;
		ptr += b;
		ptr[0] += c;
		ptr += d;
		ptr[0] += e;
		ptr += f;
	    } while(ptr[0]);
        }
        LOOP();

    _WHILEIM2:
        if(ptr[0]){
            const auto a = IP[-4].val;
            const auto b = IP[-3].val;
            const auto c = IP[-2].val;
            const auto d = IP[-1].val;
            do {
		ptr[0] += a;
		ptr += b;
		ptr[0] += c;
		ptr += d;
	    } while(ptr[0]);
        }
        LOOP();

    _M1:
        if(ptr[0]){
	    const auto a = IP[-2].val;
	    const auto b = IP[-1].val;
	    const auto c = IP->val;
	    do {
		ptr += a;
		ptr[b] += ptr[0];
		ptr[0] = 0;
		ptr += c;
	    } while(ptr[0]);
        }
	LOOP();
        
    _MOVEINCR:
        ptr += IP[-1].val;
        ptr[0] += IP->val;
        LOOP();

    _OFFINCR:
	ptr[IP[-1].val] += IP->val;
        LOOP();


    _ADVANCE:
        IP += IP->val;
        LOOP();

    _VALUE:
        LOOP();

    _ADD:
        ptr[IP->val] += (ptr[0] + 256);
        ptr[0] = 0;
	LOOP();

    _MOVE:
        ptr += IP->val;
        LOOP();
        
    _INCR:
        *ptr += IP->val;
        LOOP();
        
    _MOVEWHILE:
        while(ptr[0]){
            ptr += IP->val;
        }
        LOOP();

    _ZERO:
        ptr[0] = 0;
        LOOP();


    _PRINT:         
        std::cout << (*ptr);
        LOOP();

    _READ:
        std::cin.get(*ptr);
        LOOP();

    _TERMINATE:
        return;

    }

}

int main(int argc, char** argv)
{
    std::ifstream ifs;

    if(argc == 2){
        ifs.open(argv[1]);
        if(!ifs){
            std::cerr << "could not open " << argv[1] << std::endl;
            return 1;
        }
    }
    
    std::istream& is(argc == 2 ? ifs : std::cin);
    
    auto instrs = bf::load_instructions(is);

    bf::execute(instrs);

    return 0;
}
