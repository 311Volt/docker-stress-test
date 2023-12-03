
#ifndef CPUTESTREQUESTHANDLER_HPP
#define CPUTESTREQUESTHANDLER_HPP
#include "util.hpp"


struct CPUTestRequestHandler {
	HTTPResponse operator()() const;
};



#endif //CPUTESTREQUESTHANDLER_HPP
