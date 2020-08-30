

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>


namespace bf {

#define ENUMERATED_ACTIONS { ADVANCE,		\
			     DECR1MX,		\
			     FALSEJUMP,		\
			     GIVE,		\
			     INCR,		\
			     INCR1MX,		\
			     M1ZERO,		\
			     MOVE,		\
			     MOVEWHILE,		\
			     MXDECR1,		\
			     MXINCR1,		\
			     OFFINCR,		\
			     PRINT,		\
			     READ,		\
			     TAKE,		\
			     TERMINATE,		\
			     TRUEJUMP,		\
			     VALUE,		\
			     WHILEDECROI2,	\
			     WHILEIM2,		\
			     WHILEIM3,		\
			     WHILEMGM,		\
			     ZERO,		\
			     ZERO2IF,		\
			     ZEROM1,		}



    enum Action ENUMERATED_ACTIONS;

    struct Instruction {
	Action action;
 	short  val=0;
	void*  jump=0;
#ifdef PROFILER
	int ndx=0;
#endif
    };

    using Instructions = std::vector<Instruction>;

    std::ostream& operator<<(std::ostream& os, const Action& action);

    std::string to_string(Action);

    Instructions read_instructions(std::istream& is);

    Instructions optimizations(const Instructions& instrs, 
			       bool take_second_pass=true);

}
