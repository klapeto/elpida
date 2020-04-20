//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATIONGROUP_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATIONGROUP_HPP

#include <vector>
#include "TaskConfiguration.hpp"

namespace Elpida {
	class BenchmarkConfigurationGroup
	{
	public:

	private:
		std::vector<TaskConfiguration> _taskConfigurations;
	};
}



#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_BENCHMARKCONFIGURATIONGROUP_HPP
