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
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONSPECIFICATIONBASE_HPP

#include <string>
#include <memory>
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

		[[nodiscard]] virtual std::unique_ptr<ConfigurationValueBase> createDefault() const = 0;

		virtual ~ConfigurationSpecificationBase() = default;
	protected:
		ConfigurationSpecificationBase(std::string name,
			std::string description,
			ConfigurationType::Type type,
			bool aRequired)
			: _name(std::move(name)),
			_description(std::move(description)),
			_type(type),
			_required(aRequired)
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
