

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>


namespace bf {

#define ENUMERATED_ACTIONS { MOVE, INCR, TRUEJUMP, FALSEJUMP, ZERO, \
                  MOVEWHILE, PRINT, READ, MOVEINCR, OFFINCR, WHILEIM3, \
                  WHILEIM2, M1, ADD, ADVANCE, VALUE, TERMINATE }

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
