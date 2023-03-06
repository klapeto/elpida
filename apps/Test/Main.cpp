//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <png.h>
#include <fstream>
#include <filesystem>

#include "Benchmarks/Memory/MemoryLatencyBenchmark.hpp"
#include "Benchmarks/Memory/MemoryReadBandwidthBenchmark.hpp"
#include "Benchmarks/Image/PngEncodingDecodingBenchmark.hpp"
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

	PngEncodingDecodingBenchmark benchmark;

	EnvironmentInfo environmentInfo((OverheadsInfo()), TopologyInfo());

	std::vector<TaskConfiguration> taskConfiguration = benchmark.GetRequiredConfiguration();

	taskConfiguration[0].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9.png");
	taskConfiguration[1].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9_out.png");

	auto result = benchmark.Run(environmentInfo.GetTopologyInfo().GetAllProcessingUnits(), taskConfiguration, environmentInfo);

	auto benchmarkInfo = benchmark.GetInfo();
	std::cout << "Result: " << TranslateResult(result, benchmarkInfo) << std::endl;

	auto& taskResults = result.GetTaskResults();
	auto& taskInfos = benchmarkInfo.GetTaskInfos();
	for (std::size_t i = 0; i < taskResults.size(); ++i)
	{
		auto& taskResult = taskResults[i];
		auto& taskInfo = taskInfos[i];
		if (taskInfo.GetScoreType() == Elpida::ScoreType::Throughput)
		{
			std::cout
				<< taskInfo.GetName()
				<< ": "
				<< ValueUtilities::GetValueScaleStringSI(
					(double)taskResult.GetInputSize() / taskResult.GetDuration().count())
				<< taskInfo.GetScoreUnit()
				<< "/s"
				<< std::endl;
		}
		else
		{
			std::cout
				<< taskInfo.GetName()
				<< ": "
				<< ValueUtilities::GetValueScaleStringSI(taskResult.GetDuration().count())
				<< "s"
				<< std::endl;
		}

	}

	return EXIT_SUCCESS;
}