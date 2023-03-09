//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <dlfcn.h>

#include "Elpida/ValueUtilities.hpp"
#include "Elpida/BenchmarkResult.hpp"
#include "Elpida/BenchmarkInfo.hpp"
#include "Elpida/OverheadsInfo.hpp"
#include "Elpida/EnvironmentInfo.hpp"
#include "Elpida/TaskConfiguration.hpp"
#include "Elpida/BenchmarkGroup.hpp"

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

	EnvironmentInfo environmentInfo((OverheadsInfo()), TopologyInfo());

	auto plugin = dlopen("libelpida-memory-benchmarks.so", RTLD_LAZY);

	if (plugin == nullptr)
	{

		std::cerr << dlerror() << std::endl;
		return EXIT_FAILURE;
	}

 	auto func = (BenchmarkGroup (*)())dlsym(plugin, "GetBenchmarkGroup");

	BenchmarkGroup group = func();

	auto& benchmark = group.GetBenchmarks().front();

	std::vector<TaskConfiguration> taskConfiguration = benchmark->GetRequiredConfiguration();

	//taskConfiguration[0].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9.png");
	//taskConfiguration[1].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9_out.png");

	auto result = benchmark->Run(environmentInfo.GetTopologyInfo().GetAllProcessingUnits(), taskConfiguration, environmentInfo);

	auto benchmarkInfo = benchmark->GetInfo();
	std::cout << "Result: " << TranslateResult(result, benchmarkInfo) << std::endl;

	auto& taskResults = result.GetTaskResults();
	for (const auto& taskResult: taskResults)
	{
		auto& taskInfo = taskResult.GetTaskInfo();
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