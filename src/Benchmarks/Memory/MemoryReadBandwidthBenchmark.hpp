//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHBENCHMARK_HPP_
#define _MEMORYREADBANDWIDTHBENCHMARK_HPP_

#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class MemoryReadBandwidthBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MemoryReadBandwidthBenchmark() = default;
		~MemoryReadBandwidthBenchmark() override = default;
	 protected:
		[[nodiscard]]
		std::vector<std::unique_ptr<Task>>
		GetTasks(const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
			const std::vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const override;
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHBENCHMARK_HPP_
