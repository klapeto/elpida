//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYBENCHMARK_HPP_
#define _MEMORYLATENCYBENCHMARK_HPP_

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Benchmark.hpp"

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
