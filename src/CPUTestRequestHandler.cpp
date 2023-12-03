
#include "CPUTestRequestHandler.hpp"
#include "util.hpp"

#include <random>
#include <cmath>
#include <thread>
#include <span>

#include <crow/json.h>

void testFill(std::span<float> elements)
{
	static std::minstd_rand gen{std::random_device{}()};
	float phi = std::uniform_real_distribution<float>(0, 3.1416)(gen);
	for(int i=0; i<elements.size(); i++) {
		elements[i] = sinf(phi + i / 256.0);
	}
}

float rms(std::span<float> elements)
{
	double acc = 0.0;
	for(const auto x: elements) {
		acc += x*x;
	}
	return std::sqrt(acc / float(elements.size()));
}


struct CPUTestWorker {
	size_t blockSize;
	int repetitions;

	void operator()(float* outResult) const
	{
		std::vector<float> block(blockSize);

		for(int i=0; i<repetitions; i++) {
			testFill(block);
			*outResult = rms(block);
		}

	}
};

float rmsTest(size_t blockSize, int repetitions, int numThreads)
{
	std::vector<CPUTestWorker> workers(
		numThreads,
		CPUTestWorker {.blockSize = blockSize, .repetitions = repetitions}
	);
	std::vector<float> results(workers.size());
	{
		std::vector<std::jthread> threads;
		for(int i=0; i<workers.size(); i++) {
			threads.push_back(std::jthread{workers[i], &results[i]});
		}
	}
	return rms(results);
}

HTTPResponse CPUTestRequestHandler::operator()() const
{
	size_t totalSize = (1<<20) * 64;
	size_t blockSize = 1048576;

	int numThreads = std::thread::hardware_concurrency();
	double mb = double(totalSize) / double(1<<20);
	mb *= numThreads;

	int repetitions = totalSize/blockSize;

	auto t0 = std::chrono::high_resolution_clock::now();
	float calcResult = rmsTest(blockSize, repetitions, numThreads);
	auto t1 = std::chrono::high_resolution_clock::now();
	double ms = 1000.0 * std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();

	crow::json::wvalue result = {
		{"result", calcResult},
		{"numThreads", numThreads},
		{"dataProcessedMB", mb},
		{"elapsedTimeMs", ms}
	};
	return HTTPResponse{
		.statusCode = 200,
		.body = result.dump(),
		.contentType = "json"
	};
}
