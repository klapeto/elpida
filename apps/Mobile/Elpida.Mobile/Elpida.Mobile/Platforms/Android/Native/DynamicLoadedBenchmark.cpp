//
// Created by klapeto on 19/9/26.
//

#include "DynamicLoadedBenchmark.hpp"
#include <Elpida/Core/Config.hpp>
#include <Elpida/Core/ModuleExports.hpp>
#include <dlfcn.h>

ELPIDA_CREATE_BENCHMARK_GROUP_FUNC();

namespace Elpida
{
	Benchmark* DynamicLoadedBenchmark::GetBenchmark(uint64_t index)
	{
		if (!_benchmarkGroup)
		{
			auto func = reinterpret_cast<std::unique_ptr<BenchmarkGroup>(*)()>(dlsym(_handle, "DoCreateBenchmarkGroup"));
			if (func == nullptr)
			{
				throw std::runtime_error("Failed to resolve benchmark group creation function");
			}
			_benchmarkGroup = func();
		}
		if (index >= _benchmarkGroup->GetBenchmarks().size())
		{
			throw std::runtime_error("Invalid benchmark index. Group Size: " + std::to_string(_benchmarkGroup->GetBenchmarks().size()));
		}
		return _benchmarkGroup->GetBenchmarks()[index].get();
	}

	DynamicLoadedBenchmark::DynamicLoadedBenchmark(const char* fileName)
	{
		_handle = dlopen(fileName, RTLD_NOW);
		if (_handle == nullptr)
		{
			throw std::runtime_error(dlerror());
		}
	}

	DynamicLoadedBenchmark::~DynamicLoadedBenchmark()
	{
		_benchmarkGroup.release();
		if (_handle != nullptr)
		{
			dlclose(_handle);
		}
	}
}