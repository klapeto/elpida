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
// Created by klapeto on 8/5/20.
//

#include "Elpida/Engine/Data/DataSpecification.hpp"

#include <utility>

namespace Elpida
{

	DataSpecification::DataSpecification(std::string name, std::string unit, std::string description)
		: DataSpecification(std::move(name),
		std::move(unit),
		8,
		std::move(description))
	{

	}
	DataSpecification::DataSpecification(std::string name,
		std::string unit,
		size_t sizeShouldBeDivisibleBy,
		std::string description)
		: _name(std::move(name)),
		  _description(std::move(description)),
		  _unit(std::move(unit)),
		  _sizeShouldBeDivisibleBy(sizeShouldBeDivisibleBy)
	{

	}

	DataSpecification::DataSpecification()
		: _sizeShouldBeDivisibleBy(1)
	{

	}

	DataSpecification::DataSpecification(std::string name,
		std::string unit,
		size_t sizeShouldBeDivisibleBy,
		std::vector<std::string> requiredProperties,
		std::string description)
		: _requiredPropertiesNames(std::move(requiredProperties)),
		  _name(std::move(name)),
		  _description(std::move(description)),
		  _unit(std::move(unit)),
		  _sizeShouldBeDivisibleBy(sizeShouldBeDivisibleBy)
	{

	}

	DataSpecification::DataSpecification(std::string name,
		std::string unit,
		std::string sizeShouldBeDivisibleByProperty,
		std::string description)
		: _name(std::move(name)),
		  _description(std::move(description)),
		  _unit(std::move(unit)),
		  _sizeShouldBeDivisibleByProperty(std::move(sizeShouldBeDivisibleByProperty)),
		  _sizeShouldBeDivisibleBy(0)
	{

	}
	DataSpecification::DataSpecification(std::string name,
		std::string unit,
		std::string sizeShouldBeDivisibleByProperty,
		std::vector<std::string> requiredProperties,
		std::string description)
		: _requiredPropertiesNames(std::move(requiredProperties)),
		  _name(std::move(name)),
		  _description(std::move(description)),
		  _unit(std::move(unit)),
		  _sizeShouldBeDivisibleByProperty(std::move(sizeShouldBeDivisibleByProperty)),
		  _sizeShouldBeDivisibleBy(0)
	{

	}
}