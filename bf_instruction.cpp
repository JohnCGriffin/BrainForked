
#include "bf.hpp"

#include <sstream>

namespace bf {

    std::ostream& operator<<(std::ostream& os, const Action& action)
    {
#define ACTIONOUT(A) case A: return os << #A
	switch(action){
	    ACTIONOUT(ADVANCE);
	    ACTIONOUT(FALSEJUMP);
	    ACTIONOUT(GIVE);
	    ACTIONOUT(INCR);
	    ACTIONOUT(MXINCR1);
	    ACTIONOUT(MXDECR1);
	    ACTIONOUT(INCR1MX);
	    ACTIONOUT(DECR1MX);
	    ACTIONOUT(M1);
	    ACTIONOUT(MOVE);
	    ACTIONOUT(MOVEINCR);
	    ACTIONOUT(MOVEWHILE);
	    ACTIONOUT(OFFINCR);
	    ACTIONOUT(PRINT);
	    ACTIONOUT(READ);
	    ACTIONOUT(TAKE);
	    ACTIONOUT(TERMINATE);
	    ACTIONOUT(TRUEJUMP);
	    ACTIONOUT(VALUE);
	    ACTIONOUT(WHILEIM2);
	    ACTIONOUT(WHILEIM3);
	    ACTIONOUT(ZERO);
	    ACTIONOUT(ZEROM1);
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
