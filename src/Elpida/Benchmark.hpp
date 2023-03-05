//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARK_HPP
#define ELPIDA_BENCHMARK_HPP

#include <vector>
#include <memory>
#include <unordered_map>

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
		BenchmarkResult Run(const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
			const std::vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const;

		[[nodiscard]]
		virtual std::vector<TaskConfiguration> GetRequiredConfiguration() const = 0;

		Benchmark() = default;
		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) noexcept = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		Benchmark& operator=(Benchmark&&) = delete;
		virtual ~Benchmark() = default;
	 protected:

		[[nodiscard]]
		virtual std::vector<std::unique_ptr<Task>> GetTasks(
			const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
			const std::vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const = 0;

		[[nodiscard]]
		virtual double CalculateScore(const std::vector<TaskResult>& taskResults) const = 0;
	 private:
		static Duration ExecuteSingleThread(TaskData& data, std::unique_ptr<Task> task, const TopologyNode& topologyNode);
		static Duration ExecuteMultiThread(TaskData& data, std::unique_ptr<Task> task, const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors);

		void ValidateConfiguration(const std::vector<TaskConfiguration>& configuration) const;
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
