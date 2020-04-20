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
		[[nodiscard]] const TaskConfiguration* getConfigurationForTask(const std::string& taskKey) const;

		explicit BenchmarkConfiguration(const std::unordered_map<std::string, TaskConfiguration*>* taskConfigurations)
			: _taskConfigurations(*taskConfigurations)
		{
		}
		~BenchmarkConfiguration() = default;
	private:
		const std::unordered_map<std::string, TaskConfiguration*>& _taskConfigurations;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATION_HPP
