//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHBENCHMARK_HPP_
#define _MEMORYREADBANDWIDTHBENCHMARK_HPP_

#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class MemoryReadBandwidthBenchmark: public Benchmark
	{
	 public:
		BenchmarkInfo GetInfo() const override;
	 protected:
		std::vector<std::unique_ptr<Task>> GetTasks() const override;
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHBENCHMARK_HPP_
