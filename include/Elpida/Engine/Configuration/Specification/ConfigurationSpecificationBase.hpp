//
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP

#include <string>
#include <utility>
#include "Elpida/Engine/Configuration/ConfigurationType.hpp"

namespace Elpida
{
	class ConfigurationValueBase;
	class ConfigurationSpecificationBase
	{

	public:
		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] ConfigurationType::Type getType() const
		{
			return _type;
		}

		[[nodiscard]] bool isRequired() const
		{
			return _required;
		}

		[[nodiscard]] virtual ConfigurationValueBase* createDefault() const = 0;

		virtual ~ConfigurationSpecificationBase() = default;
	protected:
		ConfigurationSpecificationBase(std::string name,
			std::string description,
			ConfigurationType::Type type,
			bool aRequired)
			: _name(std::move(name)), _description(std::move(description)), _type(type), _required(aRequired)
		{
		}
	private:
		std::string _name;
		std::string _description;
		ConfigurationType::Type _type;
		bool _required;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP
