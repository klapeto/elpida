/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
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
// Created by klapeto on 3/5/21.
//

#ifndef ELPIDA_BENCHMARKSCORESPECIFICATION_HPP
#define ELPIDA_BENCHMARKSCORESPECIFICATION_HPP

#include <string>

namespace Elpida
{
	enum class BenchmarkScoreComparison
	{
		Lower,
		Greater
	};

	class BenchmarkScoreSpecification
	{
	public:
		[[nodiscard]]
		const std::string& getUnit() const
		{
			return _unit;
		}

		[[nodiscard]]
		BenchmarkScoreComparison getComparison() const
		{
			return _comparison;
		}

		BenchmarkScoreSpecification(std::string unit, BenchmarkScoreComparison comparison)
				: _unit(std::move(unit)), _comparison(comparison)
		{

		}

		~BenchmarkScoreSpecification() = default;

	private:
		std::string _unit;
		BenchmarkScoreComparison _comparison;
	};
}


#endif //ELPIDA_BENCHMARKSCORESPECIFICATION_HPP
