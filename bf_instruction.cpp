
#include "bf.hpp"

#include <sstream>

namespace bf {

    std::ostream& operator<<(std::ostream& os, const Action& action)
    {
#define ACTIONOUT(A) case A: return os << #A
        switch(action){
            ACTIONOUT(ADVANCE);
            ACTIONOUT(DECR1MX);
            ACTIONOUT(FALSEJUMP);
            ACTIONOUT(GIVE);
            ACTIONOUT(INCR);
            ACTIONOUT(INCR1MX);
            ACTIONOUT(M1ZERO);
            ACTIONOUT(MOVE);
            ACTIONOUT(MOVEWHILE);
            ACTIONOUT(MXDECR1);
            ACTIONOUT(MXINCR1);
            ACTIONOUT(OFFINCR);
            ACTIONOUT(PRINT);
            ACTIONOUT(READ);
            ACTIONOUT(TAKE);
            ACTIONOUT(TERMINATE);
            ACTIONOUT(TRUEJUMP);
            ACTIONOUT(VALUE);
            ACTIONOUT(WHILEDECROI2);
            ACTIONOUT(WHILEIM2);
            ACTIONOUT(WHILEIM3);
            ACTIONOUT(WHILEMGM);
            ACTIONOUT(ZERO);
            ACTIONOUT(ZERO2IF);
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
