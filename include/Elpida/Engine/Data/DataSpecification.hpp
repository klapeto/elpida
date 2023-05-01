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

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP

#include <string>
#include <vector>

namespace Elpida
{
	class DataSpecification
	{
	public:
		[[nodiscard]]
		const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]]
		size_t getSizeShouldBeDivisibleBy() const
		{
			return _sizeShouldBeDivisibleBy;
		}

		[[nodiscard]]
		const std::vector<std::string>& getRequiredPropertiesNames() const
		{
			return _requiredPropertiesNames;
		}

		[[nodiscard]]
		bool divisibleByProperty() const
		{
			return _sizeShouldBeDivisibleBy == 0;
		}

		[[nodiscard]]
		const std::string& getSizeDivisibleByPropertyName() const
		{
			return _sizeShouldBeDivisibleByProperty;
		}

		DataSpecification();
		DataSpecification(std::string name,
			std::string unit,
			std::string description);
		DataSpecification(std::string name,
			std::string unit,
			size_t sizeShouldBeDivisibleBy,
			std::string description);
		DataSpecification(std::string name,
			std::string unit,
			size_t sizeShouldBeDivisibleBy,
			std::vector<std::string> requiredProperties,
			std::string description);
		DataSpecification(std::string name,
			std::string unit,
			std::string sizeShouldBeDivisibleByProperty,
			std::string description);
		DataSpecification(std::string name,
			std::string unit,
			std::string sizeShouldBeDivisibleByProperty,
			std::vector<std::string> requiredProperties,
			std::string description);
		DataSpecification(DataSpecification&&) = default;
		DataSpecification(const DataSpecification&) = default;
		DataSpecification& operator=(DataSpecification&&) = default;
		DataSpecification& operator=(const DataSpecification&) = default;
		~DataSpecification() = default;
	private:
		std::vector<std::string> _requiredPropertiesNames;
		std::string _name;
		std::string _description;
		std::string _unit;
		std::string _sizeShouldBeDivisibleByProperty;
		size_t _sizeShouldBeDivisibleBy;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATASPECIFICATION_HPP
