//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP

#include <vector>
#include "Elpida/Engine/Configuration/ConfigurationSpecificationGroups.hpp"

namespace Elpida
{
	class Task;
	class TaskAffinity;
	class TaskSpecification;
	class BenchmarkConfiguration;
	class BenchmarkScoreCalculator;

	class Benchmark
	{
	public:
		[[nodiscard]] std::vector<TaskConfigurationSpecifications> getConfigurationSpecifications() const;
		[[nodiscard]] std::vector<Task*> createNewTasks(const TaskAffinity& affinity,
			const BenchmarkConfiguration& configuration) const;

		[[nodiscard]] size_t getTotalTasksCount() const
		{
			return _taskSpecifications.size();
		}

		[[nodiscard]] const BenchmarkScoreCalculator& getScoreCalculator() const
		{
			return _scoreCalculator;
		}

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		Benchmark(std::string name,
			std::vector<TaskSpecification*>&& taskSpecifications,
			const BenchmarkScoreCalculator& scoreCalculator);
		Benchmark(const Benchmark&) = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		virtual ~Benchmark();
	protected:
		std::vector<TaskSpecification*> _taskSpecifications;
		std::string _name;
		std::string _id;
		const BenchmarkScoreCalculator& _scoreCalculator;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
