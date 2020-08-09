
#include "bf_instruction.hpp"

namespace bf {
    
    using Instructions = std::vector<Instruction>;
    using Actions = std::initializer_list<Action>;

    static bool matches(const Instructions& instrs, int offset, Actions actions)
    {
	if(offset >= ((int) instrs.size()) - ((int) actions.size())){
	    return false;
	}
	for(auto a : actions){
            // if val cannot fit in char, deny
            if(((char) instrs.at(offset).val) != instrs.at(offset).val){
                return false;
            }
	    if(a != instrs.at(offset++).action){
		return false;
	    }
	}
	return true;
    }

    Instructions optimizations(const Instructions& instrs)
    {
	Instructions result;
	const int sz = instrs.size();

	for(int i=0; i<sz; i++){

#define AT(x) instrs.at(i+x)
#define VAL(x) instrs.at(i+x).val

	    auto pattern  = [&](Actions actions){ return matches(instrs, i, actions); };


            // common pattern of move, transfer, move
            if(pattern({ FALSEJUMP, MOVE, FALSEJUMP, INCR, MOVE, INCR, MOVE, TRUEJUMP, MOVE, TRUEJUMP}) &&
	       VAL(3) == -1 && VAL(5) == 1 && VAL(4) == -VAL(6)){
		result.push_back({ VALUE, VAL(1) });
		result.push_back({ VALUE, VAL(4) });
		result.push_back({ M1, VAL(8) });
		i += 9;
		continue;
            }
	   

	    // subtract value from current cell and add it to another cell
	    if(pattern({ FALSEJUMP, INCR, MOVE, INCR, MOVE, TRUEJUMP }) &&
	       VAL(1) == -1 && VAL(3) == 1 && VAL(2) == -VAL(4)){
		result.push_back( { ADD, VAL(2) } );
		i += 5;
		continue;
	    }

	    // subtract value from current cell and add it to another cell
	    if(pattern({ FALSEJUMP, MOVE, INCR, MOVE, INCR, TRUEJUMP }) &&
	       VAL(2) == 1 && VAL(4) == -1 && VAL(1) == -VAL(3)){
		result.push_back( { ADD, VAL(1) });
		i += 5;
		continue;
            }


            // [+>-<<<---->>>>>]
	    if(pattern({ FALSEJUMP, INCR, MOVE, INCR, MOVE, INCR, MOVE, TRUEJUMP})){
		result.push_back({ ADVANCE, 6 });
		for(int j=0; j<6; j++){
		    result.push_back({ VALUE, VAL(j+1) });
		}
		result.push_back({ WHILEIM3 });
		i += 7;
		continue;
	    }


            // [+>-<]
	    if(pattern({ FALSEJUMP, INCR, MOVE, INCR, MOVE, TRUEJUMP})){
		result.push_back({ ADVANCE, 4 });
		for(int j=0; j<4; j++){
		    result.push_back({ VALUE, VAL(j+1) });
		}
		result.push_back({ WHILEIM2 });
		i += 5;
		continue;
	    }

	    // >>+++<<
	    if(pattern({ MOVE, INCR, MOVE}) && AT(0).val == -AT(2).val){
		result.push_back({VALUE, VAL(0)});
		result.push_back({OFFINCR, VAL(1)});
		i+=2;
		continue;
	    }

	    // registers an itsy-bitsy improvement.
	    if(pattern({ MOVE, INCR})) {
		result.push_back ({VALUE, VAL(0)});
		result.push_back ({MOVEINCR, VAL(1) });
		i++;
		continue;
	    }
	    
            // +[-]
            if( pattern({ INCR, FALSEJUMP, INCR, TRUEJUMP })){
                if(result.size() && result.back().action == ZERO){
                    result.pop_back();
                }
		result.push_back({ ZERO });
		i+=3;
		continue;
            }
	    
	    // [-]
	    if(pattern({ FALSEJUMP, INCR, TRUEJUMP })){
                if(result.size() && result.back().action == ZERO){
                    result.pop_back();
                }
		result.push_back({ ZERO });
		i+=2;
		continue;
	    }

	    // move while, e.g. [<<<]
	    if(pattern({ FALSEJUMP, MOVE, TRUEJUMP })){
		result.push_back({ MOVEWHILE, VAL(1) });
		i += 2;
		continue;
	    }

	    result.push_back(instrs.at(i));
	}

	return result;
    }

}
