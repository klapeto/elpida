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

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP

#include <string>

namespace Elpida
{
	class ResultSpecification final
	{
	public:
		enum Type
		{
			Throughput,
			Raw
		};

		enum AggregationType
		{
			Accumulative,
			Average
		};

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]] AggregationType getAggregationType() const
		{
			return _aggregationType;
		}

		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] Type getType() const
		{
			return _type;
		}

		explicit ResultSpecification(std::string name,
			std::string unit,
			Type type,
			AggregationType aggregationType,
			std::string description = std::string());
		ResultSpecification(ResultSpecification&&) = default;
		ResultSpecification(const ResultSpecification&) = default;
		ResultSpecification& operator=(ResultSpecification&&) = default;
		ResultSpecification& operator=(const ResultSpecification&) = default;
		~ResultSpecification() = default;
	private:
		std::string _name;
		std::string _unit;
		std::string _description;
		Type _type;
		AggregationType _aggregationType;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_RESULT_RESULTSPECIFICATION_HPP
