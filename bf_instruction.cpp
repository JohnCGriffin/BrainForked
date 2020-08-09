
#include "bf.hpp"

#include <sstream>

namespace bf {

    using namespace std;
    
    ostream& operator<<(ostream& os, const Action& action)
    {
#define ACTIONOUT(A) case A: return os << #A
	switch(action){
            ACTIONOUT(ADVANCE);
            ACTIONOUT(VALUE);
	    ACTIONOUT(MOVE);
	    ACTIONOUT(M1);
	    ACTIONOUT(INCR);
	    ACTIONOUT(TRUEJUMP);
	    ACTIONOUT(ADD);
	    ACTIONOUT(MOVEINCR);
	    ACTIONOUT(OFFINCR);
	    ACTIONOUT(FALSEJUMP);
	    ACTIONOUT(WHILEIM2);
	    ACTIONOUT(WHILEIM3);
	    ACTIONOUT(ZERO);
	    ACTIONOUT(MOVEWHILE);
	    ACTIONOUT(PRINT);
	    ACTIONOUT(READ);
	    ACTIONOUT(TERMINATE);
	default:
	    throw logic_error(__FILE__ " unhandled Action case in operator<<");
	}
    }

    std::string to_string(Action a)
    {
	std::ostringstream oss;
	oss << a;
	return oss.str();
    }

}
