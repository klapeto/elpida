//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHBENCHMARK_HPP_
#define _MEMORYREADBANDWIDTHBENCHMARK_HPP_

#include "Elpida/Vector.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/UniquePtr.hpp"
#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class MemoryReadBandwidthBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MemoryReadBandwidthBenchmark() = default;
		~MemoryReadBandwidthBenchmark() override = default;
	 protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const override;
		double CalculateScore(const Vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHBENCHMARK_HPP_
