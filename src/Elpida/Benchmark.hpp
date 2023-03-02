//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARK_HPP
#define ELPIDA_BENCHMARK_HPP

#include <vector>
#include <memory>

#include "Elpida/BenchmarkResult.hpp"
#include "Elpida/BenchmarkInfo.hpp"
#include "Elpida/Task.hpp"

namespace Elpida {

	class OverheadsInfo;

	class Benchmark {
	public:
		[[nodiscard]]
		virtual BenchmarkInfo GetInfo() const = 0;

		[[nodiscard]]
		BenchmarkResult Run(const std::vector<int>& targetProcessorIds, const OverheadsInfo& overheadsInfo) const;

		Benchmark() = default;
		virtual ~Benchmark() = default;

	 protected:

		[[nodiscard]]
		virtual std::vector<std::unique_ptr<Task>> GetTasks() const = 0;

		[[nodiscard]]
		virtual double CalculateScore(const std::vector<TaskResult>& taskResults) const = 0;
	 private:
		static Duration ExecuteSingleThread(TaskData& data, std::unique_ptr<Task> task, int processorId);
		static Duration ExecuteMultiThread(TaskData& data, std::unique_ptr<Task> task, const std::vector<int>& targetProcessorIds);
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
