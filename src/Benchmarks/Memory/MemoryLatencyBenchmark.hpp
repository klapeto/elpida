//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYBENCHMARK_HPP_
#define _MEMORYLATENCYBENCHMARK_HPP_

#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class MemoryLatencyBenchmark : public Benchmark
	{
	 public:
		BenchmarkInfo GetInfo() const override;
	 protected:
		std::vector<std::unique_ptr<Task>> GetTasks() const override;
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYLATENCYBENCHMARK_HPP_
