/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATION_HPP

#include "ConfigurationSpecificationBase.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"
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

		[[nodiscard]] std::unique_ptr<ConfigurationValueBase> createDefault() const override
		{
			return std::make_unique<ConfigurationValue<T>>(*this, _defaultValue);
		}

		explicit ConfigurationSpecification(ConfigurationType::Bool defaultValue,
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
