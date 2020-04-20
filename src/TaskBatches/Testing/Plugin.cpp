//
// Created by klapeto on 21/4/20.
//

#include <Elpida/Utilities/Plugin/TaskBatchesContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark.hpp>

extern "C" Elpida::TaskBatchesContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = TaskBatchesContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();

	return plugin;
}
