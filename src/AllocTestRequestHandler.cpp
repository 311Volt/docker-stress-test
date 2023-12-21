
#include "AllocTestRequestHandler.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "util.hpp"

#include <crow/json.h>
// #include <sys/mman.h>

std::string performTestAllocation(size_t size)
{
	std::string result = "ok";
	try {
		[[maybe_unused]] volatile std::vector<char> test_vec(size, 113);
	} catch (std::bad_alloc& ex) {
		result = "allocation error, handled by server";
	}

	// void* mmr = mmap(nullptr, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
	// if(mmr == MAP_FAILED) {
	// 	result = std::string("mmap returned ") + std::string(strerror(errno));
	// } else {
	// 	munmap(mmr, size);
	// }

	return result;
}


HTTPResponse AllocTestRequestHandler::operator()(const std::unordered_map<std::string, std::string>& params) const
{
	if(not params.contains("size")) {
		return buildErrorResponse(400, "size parameter must be specified");
	}

	if(auto opt_size = parseSize(params.at("size"))) {

		crow::json::wvalue response_body = {
			{"allocSize", *opt_size},
			{"result", performTestAllocation(*opt_size)}
		};
		return HTTPResponse{
			.statusCode = 200,
			.body = response_body.dump(),
			.contentType = "json"
		};
	} else {
		return buildErrorResponse(400, "invalid size specifier: " + params.at("size"));
	}
}
