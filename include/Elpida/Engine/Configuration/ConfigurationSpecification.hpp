//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP

#include "ConfigurationValueBase.hpp"

namespace Elpida
{
	/**
	 * Standard information regarding a specific configuration for a task
	 */
	class ConfigurationSpecification final
	{
	public:
		const std::string& getName() const
		{
			return _name;
		}
		const std::string& getDescription() const
		{
			return _description;
		}
		ConfigurationValueBase::Type getType() const
		{
			return _type;
		}

		bool isRequired() const
		{
			return _required;
		}

		ConfigurationSpecification(std::string name,
			std::string description,
			ConfigurationValueBase::Type type, bool required)
			: _name(std::move(name)), _description(std::move(description)), _type(type), _required(required)
		{

		}
	public:
		std::string _name;
		std::string _description;
		ConfigurationValueBase::Type _type;
		bool _required;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP
