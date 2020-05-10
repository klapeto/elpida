//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP

#include <unordered_map>
#include <vector>
#include "TaskConfiguration.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{
	class TaskSpecification;
	class Benchmark;

	class BenchmarkConfiguration final
	{
	public:
		[[nodiscard]] const TaskConfiguration* getConfigurationForTask(const TaskSpecification& taskSpecification) const;
		[[nodiscard]] TaskConfiguration* getConfigurationForTask(const TaskSpecification& taskSpecification);

		const std::vector<TaskConfiguration>& getAllTaskConfigurations() const
		{
			return _orderedConfigurations;
		}

		void addConfiguration(const TaskSpecification& taskSpecification, TaskConfiguration&& configuration)
		{
			addConfigurationIml(taskSpecification, configuration);
		}

		void addConfiguration(const TaskSpecification& taskSpecification, const TaskConfiguration& configuration)
		{
			addConfigurationIml(taskSpecification, configuration);
		}

		explicit BenchmarkConfiguration(const Benchmark& benchmark);
		BenchmarkConfiguration(BenchmarkConfiguration&&) = default;
		BenchmarkConfiguration& operator=(BenchmarkConfiguration&&) = default;
		~BenchmarkConfiguration() = default;
	private:
		std::vector<TaskConfiguration> _orderedConfigurations;
		std::unordered_map<std::string, TaskConfiguration*> _taskConfigurations;
		const TaskConfiguration* getConfigurationImpl(const TaskSpecification& taskSpecification) const;

		template<typename T>
		void addConfigurationIml(const TaskSpecification& taskSpecification, T&& configuration){
			auto itr = _taskConfigurations.find(taskSpecification.getId());
			if (itr == _taskConfigurations.end())
			{
				if (_orderedConfigurations.capacity() >= _orderedConfigurations.size() + 1)
				{
					_orderedConfigurations.push_back(std::forward<T>(configuration));
					auto& cfg = _orderedConfigurations.back();
					_taskConfigurations.emplace(taskSpecification.getId(), &cfg);
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
