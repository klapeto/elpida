/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP

#include <vector>
#include "BenchmarkTaskInstance.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include <string>
#include <memory>

namespace Elpida
{
	class TaskAffinity;
	class BenchmarkConfiguration;
	class BenchmarkScoreCalculator;

	class Benchmark final
	{
	public:
		[[nodiscard]] std::vector<BenchmarkTaskInstance> createNewTasks(const TaskAffinity& affinity,
			const BenchmarkConfiguration& configuration,
			const ServiceProvider& serviceProvider) const;

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

		template <typename TSpec>
		TaskBuilder& AddTask()
		{
			_taskBuilders.push_back(TaskBuilder(std::make_shared<TSpec>()));
			return _taskBuilders.back();
		}

		template <typename TSpec>
		TaskBuilder& AddTask(std::shared_ptr<TSpec> specification)
		{
			_taskBuilders.push_back(TaskBuilder(std::move(specification)));
			return _taskBuilders.back();
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]] const std::vector<TaskBuilder>& getTaskBuilders() const
		{
			return _taskBuilders;
		}

		Benchmark(std::string name, std::shared_ptr<BenchmarkScoreCalculator> scoreCalculator);
		Benchmark(const Benchmark&) = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		~Benchmark() = default;
	protected:
		std::vector<TaskBuilder> _taskBuilders;
		std::string _name;
		std::string _id;
		std::shared_ptr<BenchmarkScoreCalculator>_scoreCalculator;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_HPP
