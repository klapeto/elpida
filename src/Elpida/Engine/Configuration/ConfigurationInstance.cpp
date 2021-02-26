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
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"

#include "Elpida/Engine/Configuration/Concrete/ConfigurationValueBase.hpp"

namespace Elpida
{

	ConfigurationInstance::ConfigurationInstance(ConfigurationInstance&& other) noexcept
	{
		move(std::move(other));
	}

	ConfigurationInstance& ConfigurationInstance::operator=(ConfigurationInstance&& other) noexcept
	{
		move(std::move(other));
		return *this;
	}

	void ConfigurationInstance::move(ConfigurationInstance&& other) noexcept
	{
		_configurationSpecification = other._configurationSpecification;
		other._configurationSpecification = nullptr;
		_value = std::move(other._value);
		other._value = nullptr;
		_fixed = other._fixed;
	}
}