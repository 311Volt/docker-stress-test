
#include "CPUTestRequestHandler.hpp"
#include "util.hpp"

#include <random>
#include <cmath>
#include <thread>
#include <span>
#include <numbers>

#include <crow/json.h>

float rms(std::span<const float> elements)
{
	double acc = 0.0;
	for(const auto x: elements) {
		acc += x*x;
	}
	return std::sqrt(acc / float(elements.size()));
}


struct TestJob {
	std::vector<float> data;

	float operator()() const
	{
		return rms(data);
	}
};

TestJob generateTestJob()
{
	static std::minstd_rand gen{std::random_device{}()};
	TestJob result;
	result.data.resize(1048576);
	for(auto& x: result.data) {
		x = sinf(std::uniform_real_distribution<float>(0, 2.0*std::numbers::pi)(gen));
	}
	return result;
}

struct CPUTestWorker {

	std::atomic<int>& numCompleted;
	std::atomic<float>& total;

	void operator()(const std::stop_token& tok) const {
		while(!tok.stop_requested()) {
			total += generateTestJob()();
			++numCompleted;
		}
	}

};

HTTPResponse CPUTestRequestHandler::operator()() const
{
	constexpr long testDurationMs = 500;

	std::atomic<int> numCompleted {};
	std::atomic<float> total {};

	int numThreads = std::thread::hardware_concurrency();
	auto t0 = std::chrono::high_resolution_clock::now();
	{
		std::vector<CPUTestWorker> workers(numThreads, CPUTestWorker{numCompleted, total});
		std::vector<std::jthread> threads;
		for(auto& worker: workers) {
			threads.emplace_back(std::jthread{worker});
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(testDurationMs));
		for(auto& thr: threads) {
			thr.request_stop();
		}
	}
	auto t1 = std::chrono::high_resolution_clock::now();

	float calcResult = total / float(numCompleted);

	using DurationDoubleMs = std::chrono::duration<double, std::milli>;

	crow::json::wvalue result = {
		{"result", calcResult},
		{"numThreads", numThreads},
		{"unitsOfWork", int(numCompleted)},
		{"elapsedTimeMs", std::chrono::duration_cast<DurationDoubleMs>(t1 - t0).count()}
	};
	return HTTPResponse{
		.statusCode = 200,
		.body = result.dump(),
		.contentType = "json"
	};
}
