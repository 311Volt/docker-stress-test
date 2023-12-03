
#include "HomepageRequestHandler.hpp"

#include <crow.h>

HTTPResponse HomepageRequestHandler::operator()() const
{
	auto page = crow::mustache::load("homepage.html");
	crow::json::wvalue ctx {};
	auto body = page.render(ctx).dump();
	return {200, body};
}
