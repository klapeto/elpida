//
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP

#include <string>
#include "ConfigurationType.hpp"

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
		[[nodiscard]] ConfigurationType getType() const
		{
			return _type;
		}
		[[nodiscard]] bool isRequired() const
		{
			return _required;
		}

		virtual ConfigurationValueBase* createDefault() const = 0;

		virtual ~ConfigurationSpecificationBase() = default;
	protected:
		ConfigurationSpecificationBase(const std::string& name,
			const std::string& description,
			ConfigurationType type,
			bool aRequired)
			: _name(name), _description(description), _type(type), _required(aRequired)
		{
		}
	private:
		std::string _name;
		std::string _description;
		ConfigurationType _type;
		bool _required;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP
