/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/TaskResult.hpp"

namespace Elpida
{
	class BenchmarkResult final
	{
	 public:

		[[nodiscard]]
		double GetResult() const
		{
			return _result;
		}

		explicit BenchmarkResult(double result)
				: _result(result)
		{
		}

		BenchmarkResult(const BenchmarkResult&) = delete;
		BenchmarkResult(BenchmarkResult&&) = default;
		~BenchmarkResult() = default;
	 private:
		double _result;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
