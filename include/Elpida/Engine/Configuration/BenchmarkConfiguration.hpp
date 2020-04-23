//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP

#include <unordered_map>
#include "TaskConfiguration.hpp"

namespace Elpida
{
	class TaskSpecification;

	class BenchmarkConfiguration final
	{
	public:
		[[nodiscard]] const TaskConfiguration* getConfigurationForTask(const TaskSpecification& taskSpecification) const;
		[[nodiscard]] TaskConfiguration* getConfigurationForTask(const TaskSpecification& taskSpecification);

		const std::unordered_map<std::string, TaskConfiguration>& getAllTaskConfigurations() const
		{
			return _taskConfigurations;
		}

		void addConfiguration(const TaskSpecification& taskSpecification, TaskConfiguration&& configuration);

		BenchmarkConfiguration() = default;
		BenchmarkConfiguration(BenchmarkConfiguration&&) = default;
		BenchmarkConfiguration& operator=(BenchmarkConfiguration&&) = default;
		~BenchmarkConfiguration() = default;
	private:
		std::unordered_map<std::string, TaskConfiguration> _taskConfigurations;
		const TaskConfiguration* getConfigurationImpl(const TaskSpecification& taskSpecification) const;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
