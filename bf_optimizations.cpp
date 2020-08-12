
#include "bf.hpp"

namespace bf {

    using Instructions = std::vector<Instruction>;
    using Actions = std::initializer_list<Action>;

    static bool matches(const Instructions& instrs, int offset, Actions actions)
    {
	const int sz = instrs.size();
	for(auto a : actions){
	    if(offset >= sz || a != instrs.at(offset++).action){
		return false;
	    }
	}
	return true;
    }

    // Two passes allows for simple forward looking pattern matches
    Instructions optimizations(const Instructions& instrs, bool take_second_pass)
    {
	Instructions result;
	const int sz = instrs.size();

	for(int i=0; i<sz; i++){

#define AT(x) instrs.at(i+x)
#define VAL(x) instrs.at(i+x).val


	    auto pattern  = [&](Actions actions){ return matches(instrs, i, actions); };


	    //////////////////////////////////////////////
	    // The following block contains optimizations
	    // for non-terminals, available in pass two.
	    //////////////////////////////////////////////

	    {
		if(pattern({ZERO, MOVE}) && VAL(1) == 1){
		    result.push_back({ ZEROM1 });
		    i++;
		    continue;
		}

		if(pattern({MOVE, ZERO}) && VAL(0) == 1){
		    result.push_back({ M1ZERO });
		    i++;
		    continue;
		}

		if(pattern({FALSEJUMP, MOVE, ZERO, MXDECR1, TRUEJUMP }) &&
		   VAL(1) == 1 && VAL(3) == -1){
		    result.push_back({ZERO2IF});
		    i+=4;
		    continue;
		}

		if(pattern({ MOVE, GIVE, MOVE}) &&
		   VAL(2) == VAL(1) &&
		   VAL(0) == -VAL(2)){
		    result.push_back({ TAKE, VAL(0) });
		    i+=2;
		    continue;
		}

		if(pattern({FALSEJUMP, MOVE, GIVE, MOVE, TRUEJUMP})){
		    result.push_back({VALUE, VAL(1) });
		    result.push_back({VALUE, VAL(2) });
		    result.push_back({WHILEMGM, VAL(3) });
		    i += 4;
		    continue;
		}

                if(pattern({DECR1MX, GIVE, MOVE}) &&
		   VAL(0) == -VAL(1) && VAL(0) == -VAL(2)){
		    result.push_back({INCR, -1});
		    result.push_back({TAKE, VAL(0)});
		    i += 2;
		    continue;
		}
            
	    }


            ///////////////////////////////////////////////////
            //  All that follow are primitives for first pass.
            //////////////////////////////////////////////////
	    {
		// subtract value from current cell and add it to another cell
		if(pattern({ FALSEJUMP, INCR, MOVE, INCR, MOVE, TRUEJUMP }) &&
		   VAL(1) == -1 && VAL(3) == 1 && VAL(2) == -VAL(4)){
		    result.push_back( { GIVE, VAL(2) } );
		    i += 5;
		    continue;
		}

		// alternate version decrements last
		// subtract value from current cell and add it to another cell
		if(pattern({ FALSEJUMP, MOVE, INCR, MOVE, INCR, TRUEJUMP }) &&
		   VAL(2) == 1 && VAL(4) == -1 && VAL(1) == -VAL(3)){
		    result.push_back( { GIVE, VAL(1) });
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

		if(pattern({ INCR, MOVE}) && VAL(0) == -1){
		    result.push_back({ DECR1MX, VAL(1) });
		    i++;
		    continue;
		}

		if(pattern({ INCR, MOVE}) && VAL(0) == 1){
		    result.push_back({ INCR1MX, VAL(1) });
		    i++;
		    continue;
		}

		if(pattern({ MOVE, INCR }) && VAL(1) == 1){
		    result.push_back({ MXINCR1, VAL(0) });
		    i++;
		    continue;
		}

		if(pattern({ MOVE, INCR }) && VAL(1) == -1){
		    result.push_back({ MXDECR1, VAL(0) });
		    i++;
		    continue;
		}

		// [-]
		if(pattern({ FALSEJUMP, INCR, TRUEJUMP })){
		    while(result.back().action == ZERO || result.back().action == INCR){
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
	    }

	    result.push_back(instrs.at(i));
	}

        if(take_second_pass){
            result = optimizations(result,false);
        }

	return result;
    }

}
