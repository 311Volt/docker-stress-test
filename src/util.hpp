
#ifndef UTIL_HPP
#define UTIL_HPP

#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <crow/json.h>

struct HTTPResponse {
	int statusCode;
	std::string body;
	std::string contentType = "html";
};

static inline std::unordered_map<int, std::string> httpStatusCodes = {
	{200, "OK"},
	{400, "Bad Request"},
	{401, "Forbidden"},
	{404, "Not Found"},
	{500, "Internal Server Error"}
};

/* parses a number, possibly suffixed, into the number of bytes.
 * examples:
 * "3k" -> 3072
 * "1M" -> 1048576
 * "20" -> 20
 * "1x" -> std::nullopt */
inline std::optional<size_t> parseSize(const std::string& size_str)
{
	try {
		size_t pos = 0;
		size_t result = std::stoul(size_str, &pos);
		switch(size_str[pos]) {
			case '\0': break;
			case 'k': result <<= 10; break;
			case 'M': result <<= 20; break;
			case 'G': result <<= 30; break;
			default: throw std::invalid_argument("invalid suffix");
		}
		return result;
	} catch(std::exception&) {
		return {};
	}
}

inline auto buildErrorResponse(int code, const std::string& message)
{
	auto statusName = httpStatusCodes.contains(code) ? httpStatusCodes.at(code) : "";
	crow::json::wvalue res {
		{"status", statusName},
		{"error", message}
	};
	return HTTPResponse {
		.statusCode = code,
		.body = res.dump(),
		.contentType = "json"
	};
}


#endif //UTIL_HPP
