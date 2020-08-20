

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>


namespace bf {

#define ENUMERATED_ACTIONS { ADVANCE, FALSEJUMP, GIVE, INCR, MOVE, WHILEDECROI2, \
			     MOVEWHILE, OFFINCR, PRINT, READ, TAKE, TERMINATE, \
			     M1ZERO, MXINCR1, MXDECR1, INCR1MX, DECR1MX, TRUEJUMP, \
                             WHILEMGM, VALUE, WHILEIM2, WHILEIM3, ZERO, ZERO2IF, ZEROM1 }

    enum Action ENUMERATED_ACTIONS;

    struct Instruction {
	Action action;
        short val;
	void* jump;
#ifdef PROFILER
        int ndx;
#endif
    };

    using Instructions = std::vector<Instruction>;

    std::ostream& operator<<(std::ostream& os, const Action& action);

    std::string to_string(Action);

    Instructions read_instructions(std::istream& is);

    Instructions optimizations(const Instructions& instrs, 
        bool take_second_pass=true);

}
