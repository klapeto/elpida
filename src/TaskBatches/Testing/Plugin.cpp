//
// Created by klapeto on 21/4/20.
//

#include <Elpida/Utilities/Plugin/TaskBatchesContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/DefaultBenchmarkScoreCalculator.hpp>
#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>
#include <Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp>

extern "C" Elpida::TaskBatchesContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = TaskBatchesContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();

	auto benchmark = new Benchmark("Test Benchmark",{
		new ReadFileSpecification(true, false, "/media/klapeto/Αρχεία/Isos/neon-user-20200326-1117.iso"),
		new WriteFileSpecification(true, false,"/media/klapeto/Αρχεία/Isos/TEST.iso"),
	}, new DefaultBenchmarkScoreCalculator());

	plugin->add(benchmark);

	benchmark = new Benchmark("Test Allocate Memory",{
		new AllocateMemorySpecification(true, false, true)
	}, new DefaultBenchmarkScoreCalculator());

	plugin->add(benchmark);

	return plugin;
}
