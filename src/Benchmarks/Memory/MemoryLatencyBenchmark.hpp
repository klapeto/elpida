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
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		MemoryLatencyBenchmark() = default;
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;
		~MemoryLatencyBenchmark() override = default;
	 protected:
		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(
			const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
			const std::vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYLATENCYBENCHMARK_HPP_
