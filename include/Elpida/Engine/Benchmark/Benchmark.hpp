//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP

#include <vector>
#include "Elpida/Engine/Configuration/Specification/TaskConfigurationSpecifications.hpp"
#include "BenchmarkTaskInstance.hpp"
#include <string>

namespace Elpida
{
	class TaskAffinity;
	class BenchmarkConfiguration;
	class BenchmarkScoreCalculator;
	class TaskBuilder;

	class Benchmark
	{
	public:
		[[nodiscard]] std::vector<BenchmarkTaskInstance> createNewTasks(const TaskAffinity& affinity,
			const BenchmarkConfiguration& configuration) const;

		[[nodiscard]] size_t getTotalTasksCount() const
		{
			return _taskBuilders.size();
		}

		[[nodiscard]] const BenchmarkScoreCalculator& getScoreCalculator() const
		{
			return *_scoreCalculator;
		}

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]] const std::vector<TaskBuilder*>& getTaskBuilders() const
		{
			return _taskBuilders;
		}

		Benchmark(std::string name,
			std::vector<TaskBuilder*>&& taskSpecifications,
			BenchmarkScoreCalculator* scoreCalculator);
		Benchmark(const Benchmark&) = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		virtual ~Benchmark();
	protected:
		std::vector<TaskBuilder*> _taskBuilders;
		std::string _name;
		std::string _id;
		BenchmarkScoreCalculator* _scoreCalculator;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
