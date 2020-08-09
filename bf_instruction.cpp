
#include "bf.hpp"

#include <sstream>

namespace bf {

    std::ostream& operator<<(std::ostream& os, const Action& action)
    {
#define ACTIONOUT(A) case A: return os << #A
	switch(action){
	    ACTIONOUT(ADD);
	    ACTIONOUT(FALSEJUMP);
	    ACTIONOUT(INCR);
	    ACTIONOUT(M1);
	    ACTIONOUT(MOVE);
	    ACTIONOUT(MOVEINCR);
	    ACTIONOUT(MOVEWHILE);
	    ACTIONOUT(OFFINCR);
	    ACTIONOUT(PRINT);
	    ACTIONOUT(READ);
	    ACTIONOUT(TERMINATE);
	    ACTIONOUT(TRUEJUMP);
	    ACTIONOUT(WHILEIM2);
	    ACTIONOUT(WHILEIM3);
	    ACTIONOUT(ZERO);
	    ACTIONOUT(ADVANCE);
	    ACTIONOUT(VALUE);
	default:
	    throw std::logic_error(__FILE__ " unhandled Action case in operator<<");
	}
    }

    std::string to_string(Action a)
    {
	std::ostringstream oss;
	oss << a;
	return oss.str();
    }

}
