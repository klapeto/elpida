//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include <execinfo.h>

#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/BenchmarkResult.hpp"
#include "Elpida/Core/BenchmarkInfo.hpp"
#include "Elpida/Core/OverheadsInfo.hpp"
#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/DefaultAllocator.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "Elpida/Platform/NumaAllocator.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"

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

void
handler()
{
	void *trace_elems[20];
	int trace_elem_count(backtrace( trace_elems, 20 ));
	char **stack_syms(backtrace_symbols( trace_elems, trace_elem_count ));
	for ( int i = 0 ; i < trace_elem_count ; ++i )
	{
		std::cout << stack_syms[i] << "\n";
	}
	free( stack_syms );

	exit(1);
}

std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
{
	return ValueUtilities::GetValueScaleStringSI(result.GetScore()) + benchmarkInfo.GetScoreUnit();
}

int main(int argC, char* argV[])
{
	//std::set_terminate(handler);
	TopologyLoader topologyLoader;
	EnvironmentInfo environmentInfo((OverheadsInfo()), topologyLoader.LoadTopology(), std::make_unique<NumaAllocator>());

	BenchmarkGroupModule externalBenchmarkGroup("/home/klapeto/code/elpida/AppDir/bin/Benchmarks/libelpida-memory-benchmarks.so");

	auto& benchmark = externalBenchmarkGroup.GetBenchmarkGroup().GetBenchmarks().back();

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