//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONGROUPS_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONGROUPS_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace Elpida
{
	class ConfigurationSpecification;
	class ConfigurationSpecificationGroups final
	{
	public:

		const std::unordered_map<std::string, std::vector<ConfigurationSpecification*>>& getGroups() const
		{
			return _groups;
		}

		ConfigurationSpecificationGroups(std::unordered_map<std::string,
															std::vector<ConfigurationSpecification*>>&& groups)
			: _groups(std::move(groups))
		{

		};
		~ConfigurationSpecificationGroups() = default;
	private:
		std::unordered_map<std::string, std::vector<ConfigurationSpecification*>> _groups;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONGROUPS_HPP
