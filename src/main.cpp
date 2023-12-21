
#include "AllocTestRequestHandler.hpp"
#include "CPUTestRequestHandler.hpp"
#include "HomepageRequestHandler.hpp"
#include "nothinginteresting.hpp"

#include <crow.h>
#include <concepts>
#include <unordered_map>

template<typename HandlerT>
struct CrowRequestHandlerWrapper {
	HandlerT handler;

	static crow::response convertResponse(const HTTPResponse& response)
	{
		return {response.statusCode, response.contentType, response.body};
	}

	crow::response operator()(const crow::request& req) const
	{
		if constexpr (std::invocable<HandlerT, std::unordered_map<std::string, std::string>>) {
			std::unordered_map<std::string, std::string> dict;
			for(const auto& key: req.url_params.keys()) {
				dict[key] = req.url_params.get(key);
			}
			return convertResponse(handler(dict));
		} else {
			return convertResponse(handler());
		}
	}
};


template<typename HandlerT>
auto wrapHandler(HandlerT&& handler)
{
	return CrowRequestHandlerWrapper<HandlerT>{.handler = std::forward<HandlerT>(handler)};
}

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/api/cpu")(wrapHandler(CPUTestRequestHandler {}));
	CROW_ROUTE(app, "/api/alloc")(wrapHandler(AllocTestRequestHandler {}));
	CROW_ROUTE(app, "/")(wrapHandler(HomepageRequestHandler {}));
	CROW_ROUTE(app, "/no-easter-eggs-here-i-swear")(wrapHandler(TestHandler {}));

	app.port(20311).multithreaded().run();
}

