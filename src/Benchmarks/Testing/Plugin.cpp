//
// Created by klapeto on 21/4/20.
//

#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/DefaultBenchmarkScoreCalculator.hpp>
#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>
#include <Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>

extern "C" Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = BenchmarksContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();
	//"/media/klapeto/Αρχεία/Isos/neon-user-20200326-1117.iso"
	//"/media/klapeto/Αρχεία/Isos/TEST.iso"


	auto& readFile = (new TaskBuilder(*new ReadFileSpecification))
		->shouldBeCountedOnResults()
		.canBeMultiThreaded(false)
		.canBeDisabled(false);

	auto& writeFile = (new TaskBuilder(*new WriteFileSpecification))
		->shouldBeCountedOnResults()
		.canBeMultiThreaded(false)
		.canBeDisabled(false);

	auto benchmark = new Benchmark("Test Benchmark", {
		&readFile,
		&writeFile,
	}, new DefaultBenchmarkScoreCalculator());

	plugin->add(benchmark);

//	benchmark = new Benchmark("Test Allocate Memory", {
//		new AllocateMemorySpecification()
//	}, new DefaultBenchmarkScoreCalculator());
//
//	plugin->add(benchmark);

	return plugin;
}
