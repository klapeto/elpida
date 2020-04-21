//
// Created by klapeto on 21/4/20.
//

#include <Elpida/Utilities/Plugin/TaskBatchesContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark.hpp>
#include <Elpida/Engine/DefaultBenchmarkScoreCalculator.hpp>
#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>

extern "C" Elpida::TaskBatchesContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = TaskBatchesContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();

	auto benchmark = new Benchmark("Test Benchmark",{
		new ReadFileSpecification(true, false),
		new WriteFileSpecification(true, false)
	}, DefaultBenchmarkScoreCalculator());

	plugin->add(benchmark);

	return plugin;
}
