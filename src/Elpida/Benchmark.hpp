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

	class Benchmark {
	public:
		virtual BenchmarkInfo GetInfo() const = 0;
		BenchmarkResult Run() const;
	 protected:
		virtual std::vector<std::unique_ptr<Task>> GetTasks() const = 0;
		virtual double CalculateScore(const std::vector<TaskResult>& taskResults) const = 0;
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
