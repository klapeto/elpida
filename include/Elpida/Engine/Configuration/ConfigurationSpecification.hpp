//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP

#include "ConfigurationSpecificationBase.hpp"
#include "ConfigurationValue.hpp"
#include "ConfigurationType.hpp"
#include <string>

namespace Elpida
{
	class ConfigurationValueBase;

	/**
	 * Standard information regarding a specific configuration for a task
	 */

	template<typename T>
	class ConfigurationSpecification : public ConfigurationSpecificationBase
	{
	public:

		ConfigurationValueBase* createDefault() const override
		{
			return new ConfigurationValue<T>(*this, _defaultValue);
		}

		explicit ConfigurationSpecification(bool defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Type::Bool, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_same_v<ConfigurationType::Bool, T>, "ConfigurationValue is not specified as bool");
		}

		explicit ConfigurationSpecification(ConfigurationType::UnsignedInt defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Type::UnsignedInt, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as unsigned integral");
		}

		explicit ConfigurationSpecification(ConfigurationType::Int defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Type::Int, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as integral");
		}

		explicit ConfigurationSpecification(ConfigurationType::Float defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Type::Float, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_floating_point_v<T>, "ConfigurationValue is not specified as floating point");
		}

		explicit ConfigurationSpecification(const ConfigurationType::String& defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Type::String, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_convertible_v<std::string, T>, "ConfigurationValue is not specified as string");
		}

		ConfigurationSpecification(ConfigurationType::Type type,
			T defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, type, isRequired), _defaultValue(defaultValue)
		{
		}

		~ConfigurationSpecification() override = default;
	private:
		T _defaultValue;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP
