//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "Elpida/Test/MemoryLatencyBenchmark.hpp"
#include "Elpida/Test/MemoryReadBandwidthBenchmark.hpp"
#include "Elpida/ValueUtilities.hpp"
#include "Elpida/OverheadsInfo.hpp"

using namespace Elpida;

static void Load()
{
	std::srand(std::time(nullptr));
	auto salt = (std::size_t)(std::rand() / (float)(RAND_MAX) * 10);
	std::size_t x = 1000000000;
	auto f = 0.0f;
	while (x--)
	{
		f += (float)x / (float)(salt + x);
	}
	std::cout << "Done with: " << f << std::endl;
}

std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
{
	return ValueUtilities::GetValueScaleStringSI(result.GetScore()) + benchmarkInfo.GetScoreUnit();
}

int main(int argC, char* argV[])
{
	MemoryReadBandwidthBenchmark benchmark;

	OverheadsInfo overheadsInfo{};

	//Load();

	auto result = benchmark.Run({ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 }, overheadsInfo);

	std::cout << "Result: " << TranslateResult(result, benchmark.GetInfo()) << std::endl;

	return EXIT_SUCCESS;
}