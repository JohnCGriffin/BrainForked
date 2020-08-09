

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>


namespace bf {

#define ENUMERATED_ACTIONS { ADD, ADVANCE, FALSEJUMP, INCR, M1, MOVE, MOVEINCR, \
			     MOVEWHILE, OFFINCR, PRINT, READ, TERMINATE, \
			     TRUEJUMP, VALUE, WHILEIM2, WHILEIM3, ZERO }

    enum Action ENUMERATED_ACTIONS;

    struct Instruction {
	Action action;
        short val;
	void* jump;
    };

    using Instructions = std::vector<Instruction>;

    std::ostream& operator<<(std::ostream& os, const Action& action);

    std::string to_string(Action);

    Instructions read_instructions(std::istream& is);

    Instructions optimizations(const Instructions& instrs);

}
