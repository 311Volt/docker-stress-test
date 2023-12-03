
#ifndef ALLOCTESTREQUESTHANDLER_HPP
#define ALLOCTESTREQUESTHANDLER_HPP

#include "util.hpp"


struct AllocTestRequestHandler {
	HTTPResponse operator()(const std::unordered_map<std::string, std::string>&params) const;
};



#endif //ALLOCTESTREQUESTHANDLER_HPP
