//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYBENCHMARK_HPP_
#define _MEMORYLATENCYBENCHMARK_HPP_

#include "Elpida/Vector.hpp"
#include "Elpida/UniquePtr.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class MemoryLatencyBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		MemoryLatencyBenchmark() = default;
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;
		~MemoryLatencyBenchmark() override = default;
	 protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const override;

		[[nodiscard]]
		double CalculateScore(const Vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_MEMORYLATENCYBENCHMARK_HPP_
