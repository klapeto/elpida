//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP

#include <unordered_map>

namespace Elpida
{
	class TaskConfiguration;
	class TaskSpecification;

	class BenchmarkConfiguration final
	{
	public:
		[[nodiscard]] TaskConfiguration* getConfigurationForTask(const TaskSpecification& taskSpecification) const;

		void addConfiguration(const TaskSpecification& taskSpecification, TaskConfiguration& configuration);

		BenchmarkConfiguration() = default;
		~BenchmarkConfiguration() = default;
	private:
		std::unordered_map<std::string, TaskConfiguration*> _taskConfigurations;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
