//
// Created by klapeto on 19/9/26.
//

#ifndef ELPIDA_DYNAMICLIBRARY_HPP
#define ELPIDA_DYNAMICLIBRARY_HPP

#include <new>
#include <Elpida/Core/BenchmarkGroup.hpp>
#include <Elpida/Core/Benchmark.hpp>

namespace Elpida
{
	class DynamicLoadedBenchmark
	{
	public:
		Benchmark* GetBenchmark(uint64_t index);

		DynamicLoadedBenchmark(const char* fileName);
		DynamicLoadedBenchmark(const DynamicLoadedBenchmark&) = delete;
		DynamicLoadedBenchmark(DynamicLoadedBenchmark&&) = delete;
		DynamicLoadedBenchmark& operator=(const DynamicLoadedBenchmark&) = delete;
		DynamicLoadedBenchmark& operator=(DynamicLoadedBenchmark&&) = delete;
		~DynamicLoadedBenchmark();
	private:
		std::unique_ptr<BenchmarkGroup> _benchmarkGroup;
		void* _handle;
	};

}

#endif //ELPIDA_DYNAMICLIBRARY_HPP
