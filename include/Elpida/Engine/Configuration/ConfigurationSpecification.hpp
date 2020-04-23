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
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Bool, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_same_v<bool, T>, "ConfigurationValue is not specified as bool");
		}

		explicit ConfigurationSpecification(unsigned long defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::UnsignedInt, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as unsigned integral");
		}

		explicit ConfigurationSpecification(long defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Int, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as integral");
		}

		explicit ConfigurationSpecification(double defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::Float, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_floating_point_v<T>, "ConfigurationValue is not specified as floating point");
		}

		explicit ConfigurationSpecification(const std::string& defaultValue,
			const std::string& name,
			const std::string& description,
			bool isRequired)
			: ConfigurationSpecificationBase(name, description, ConfigurationType::String, isRequired),
			  _defaultValue(defaultValue)
		{
			static_assert(std::is_convertible_v<std::string, T>, "ConfigurationValue is not specified as string");
		}

		ConfigurationSpecification(ConfigurationType type,
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
