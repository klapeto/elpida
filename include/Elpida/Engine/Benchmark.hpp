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

	class Benchmark final
	{
	public:

		[[nodiscard]] ConfigurationSpecificationGroups getConfigurationSpecifications() const;
		[[nodiscard]] std::vector<Task*> createNewTasks(const TaskAffinity& affinity, const BenchmarkConfiguration& configuration);

		[[nodiscard]] static std::string getKeyForTask(const TaskSpecification& specification, size_t index);

		[[nodiscard]] size_t getTotalTasksCount() const
		{
			return _taskSpecifications.size();
		}

		[[nodiscard]] const BenchmarkScoreCalculator& getScoreCalculator() const
		{
			return _scoreCalculator;
		}

		Benchmark(std::string name,
			std::vector<TaskSpecification*>&& taskSpecifications,
			const BenchmarkScoreCalculator& scoreCalculator);
		~Benchmark();
	protected:
		std::vector<TaskSpecification*> _taskSpecifications;
		std::string _name;
		const BenchmarkScoreCalculator& _scoreCalculator;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
