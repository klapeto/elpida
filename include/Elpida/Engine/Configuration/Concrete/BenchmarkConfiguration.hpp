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
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP

#include <unordered_map>
#include <vector>
#include "TaskConfiguration.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{
	class Benchmark;

	class BenchmarkConfiguration final
	{
	public:
		[[nodiscard]]
		std::optional<std::reference_wrapper<const TaskConfiguration>>
		getConfigurationForTask(const TaskBuilder& taskBuilder) const;

		[[nodiscard]]
		std::optional<std::reference_wrapper<TaskConfiguration>>
		getConfigurationForTask(const TaskBuilder& taskBuilder);

		const std::vector<TaskConfiguration>& getAllTaskConfigurations() const
		{
			return _orderedConfigurations;
		}

		void addConfiguration(const TaskBuilder& taskSpecification, TaskConfiguration&& configuration)
		{
			addConfigurationIml(taskSpecification, std::move(configuration));
		}

		void addConfiguration(const TaskBuilder& taskSpecification, const TaskConfiguration& configuration)
		{
			addConfigurationIml(taskSpecification, configuration);
		}

		explicit BenchmarkConfiguration(const Benchmark& benchmark);

		BenchmarkConfiguration(BenchmarkConfiguration&&) = default;

		BenchmarkConfiguration& operator=(BenchmarkConfiguration&&) = default;

		BenchmarkConfiguration(const BenchmarkConfiguration&) = delete;

		BenchmarkConfiguration& operator=(const BenchmarkConfiguration&) = delete;

		~BenchmarkConfiguration() = default;

	private:
		std::vector<TaskConfiguration> _orderedConfigurations;
		std::unordered_map<std::string, TaskConfiguration*> _taskConfigurations;

		TaskConfiguration* getConfigurationImpl(const TaskBuilder& taskBuilder) const;

		template<typename T>
		void addConfigurationIml(const TaskBuilder& taskBuilder, T&& configuration)
		{
			auto itr = _taskConfigurations.find(taskBuilder.getId());
			if (itr == _taskConfigurations.end())
			{
				if (_orderedConfigurations.capacity() >= _orderedConfigurations.size() + 1)
				{
					_orderedConfigurations.push_back(std::forward<T>(configuration));
					auto& cfg = _orderedConfigurations.back();
					_taskConfigurations.emplace(taskBuilder.getId(), &cfg);
				}
				else
				{
					throw ElpidaException(FUNCTION_NAME,
							"Attempted to add more Task Configurations than the benchmark has");
				}
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME,
						"Attempted to add a configuration that already exists");
			}
		}
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
