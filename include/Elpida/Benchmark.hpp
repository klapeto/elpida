//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARK_HPP
#define ELPIDA_BENCHMARK_HPP

#include "Elpida/Vector.hpp"
#include "Elpida/BenchmarkResult.hpp"
#include "Elpida/BenchmarkInfo.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/Topology/ProcessingUnitNode.hpp"
#include "Elpida/EnvironmentInfo.hpp"

namespace Elpida {

	class OverheadsInfo;

	class Benchmark {
	public:
		[[nodiscard]]
		virtual BenchmarkInfo GetInfo() const = 0;

		[[nodiscard]]
		BenchmarkResult Run(
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const;

		[[nodiscard]]
		virtual Vector<TaskConfiguration> GetRequiredConfiguration() const = 0;

		Benchmark() = default;
		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) noexcept = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		Benchmark& operator=(Benchmark&&) = delete;
		virtual ~Benchmark() = default;
	 protected:

		[[nodiscard]]
		virtual Vector<UniquePtr<Task>> GetTasks(
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const = 0;

		[[nodiscard]]
		virtual double CalculateScore(const Vector<TaskResult>& taskResults) const = 0;
	 private:
		static Duration ExecuteSingleThread(UniquePtr<AbstractTaskData>& data, UniquePtr<Task> task, const TopologyNode& topologyNode);
		static Duration ExecuteMultiThread(UniquePtr<AbstractTaskData>& data, UniquePtr<Task> task, const Vector<Ref<const ProcessingUnitNode>>& targetProcessors);

		void ValidateConfiguration(const Vector<TaskConfiguration>& configuration) const;
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
