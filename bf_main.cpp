
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <map>
#include <cstring>

#include "bf.hpp"

#ifdef PROFILER
#include <thread>
#endif


#ifdef PROFILER
static volatile int current_loop;
static std::map<volatile int,unsigned long> counts;
#endif

namespace bf {

    static void patch_while_loops(Instructions& instructions)
    {
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
    }

    static Instructions load_instructions(std::istream& is)
    {
        auto instructions = read_instructions(is);

        instructions = optimizations(instructions);

        patch_while_loops(instructions);

        instructions.push_back({ TERMINATE });

        return instructions;
    }

    
    static void execute(Instructions&  _instructions)
    {
        std::map<Action,void*> addrs;

        // Note that initialization in modern manner chokes clang/g++ 
        // when in c++14/17.  Direct assignment one by one seems to work.
	addrs[ZEROM1]      = &&_ZEROM1;
        addrs[ADVANCE]     = &&_ADVANCE;
        addrs[DECR1MX]     = &&_DECR1MX;
        addrs[FALSEJUMP]   = &&_FALSEJUMP;
        addrs[GIVE]        = &&_GIVE;
        addrs[INCR1MX]     = &&_INCR1MX;
        addrs[INCR]        = &&_INCR;
	addrs[M1ZERO]      = &&_M1ZERO;
        addrs[MOVEWHILE]   = &&_MOVEWHILE;
        addrs[WHILEMGM]    = &&_WHILEMGM;
	addrs[WHILEDECROI2]= &&_WHILEDECROI2;
        addrs[MOVE]        = &&_MOVE;
        addrs[MXDECR1]     = &&_MXDECR1;
        addrs[MXINCR1]     = &&_MXINCR1;
        addrs[OFFINCR]     = &&_OFFINCR;
        addrs[PRINT]       = &&_PRINT;
        addrs[READ]        = &&_READ;
        addrs[TAKE]        = &&_TAKE;
        addrs[TERMINATE]   = &&_TERMINATE;
        addrs[TRUEJUMP]    = &&_TRUEJUMP;
        addrs[VALUE]       = &&_VALUE;
        addrs[WHILEIM2]    = &&_WHILEIM2;
        addrs[WHILEIM3]    = &&_WHILEIM3;
        addrs[ZERO]        = &&_ZERO;
	addrs[ZERO2IF]     = &&_ZERO2IF;

	for(auto a : ENUMERATED_ACTIONS){
            if(!addrs[a]){
		throw std::logic_error(to_string(a) + " not in address table");
            }
        }

        for(auto& instr : _instructions){
            instr.jump = addrs.at(instr.action);
        }

#ifdef PROFILER
        for(int i=0; i<_instructions.size(); i++){
            _instructions.at(i).ndx = i;
        }
#endif

        std::vector<char> characters(30000);
        char* ptr = characters.data();
        auto BEGIN = _instructions.data();
        auto IP = BEGIN;

        // initial JUMP.  The remainder happen via the LOOP macro.
        goto *(IP->jump);

#define LOOP() IP++; goto *(IP->jump)

    _TRUEJUMP:
        if(*ptr){
            IP += IP->val;
        }
        LOOP();
        
    _FALSEJUMP:     
        if(!(*ptr)){
            IP += IP->val;
        }
#ifdef PROFILER
	else {
	    current_loop = IP->ndx;
	}
#endif
        LOOP();

    _DECR1MX:
        ptr[0]--;
        ptr += IP->val;
        LOOP();

    _INCR1MX:
        ptr[0]++;
        ptr += IP->val;
        LOOP();

    _MXINCR1:
        ptr += IP->val;
        ptr[0]++;
        LOOP();

    _MXDECR1:
        ptr += IP->val;
        ptr[0]--;
        LOOP();

    _WHILEDECROI2:
	if(ptr[0]){
	    const int o1 = IP[-2].val;
	    const int o2 = IP[-1].val;
	    do {
		ptr[0]--;
		ptr[o1]++;
		ptr[o2]++;
	    } while(ptr[0]);
	}
	LOOP();
  
    _WHILEMGM:
        if(ptr[0]){
	    const int m1 = IP[-2].val;
	    const int g = IP[-1].val;
	    const int m2 = IP->val;
	    do {
		ptr += m1;
		ptr[g] += ptr[0];
		ptr[0] = 0;
		ptr += m2;
	    } while(ptr[0]);
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

    _OFFINCR:
	ptr[IP[-1].val] += IP->val;
        LOOP();


    _ADVANCE:
        IP += IP->val;
        LOOP();

    _VALUE:
        LOOP();

    _GIVE:
        ptr[IP->val] += (ptr[0] + 256);
        ptr[0] = 0;
	LOOP();

    _TAKE:
        ptr[0] += (ptr[IP->val] + 256);
        ptr[IP->val] = 0;
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

    _ZERO2IF:
	if(ptr[0]){
	    ptr[0] = ptr[1] = 0;
	}
	LOOP();

    _ZEROM1:
	*ptr++ = 0;
	LOOP();

    _M1ZERO:
	*(++ptr) = 0;
	LOOP();

    _ZERO:
        ptr[0] = 0;
        LOOP();


    _PRINT:
#ifndef PROFILER
        std::cout << (*ptr);
#endif
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
    bool dump = argc > 1 && !strcmp("-dump",argv[1]);
    if(dump){
        argc--;
        argv++;
    }
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

    if(dump){
        auto while_level = 0;
        for(int i=0; i<instrs.size(); i++){
	    auto instr = instrs[i];
            if(instr.action == bf::TRUEJUMP){
		while_level--;
	    }
            std::cout.width(6);
            std::cout << i << " ";
	    for(int j=0; j<while_level; j++){
		std::cout << "  ";
	    }
	    std::cout << instr.action << " " << instr.val << "\n";
	    if(instr.action == bf::FALSEJUMP){
		while_level++;
	    }
        }
	return 0;
    }

#ifdef PROFILER

    std::thread profiler([&]{
			     while(!current_loop) ;
			     while(current_loop){
				 auto count = counts[current_loop];
				 if(count > 500000){
				     break;
				 }
				 counts[current_loop]++;
				 std::this_thread::yield();
				 std::cout << std::flush;
			     }
			 });

    bf::execute(instrs);
    current_loop = 0;

    profiler.join();

    for(auto entry : counts){
	std::cout << entry.second << " " << entry.first 
		  << " " << instrs.at(entry.first).val + entry.first << "\n";
    }
#else

    bf::execute(instrs);

#endif

    return 0;
}
