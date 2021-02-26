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
// Created by klapeto on 29/6/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKGROUP_HPP
#define APPS_QT_MODELS_BENCHMARKGROUP_HPP

#include <string>
#include <vector>

#include <Elpida/Engine/Benchmark/Benchmark.hpp>

namespace Elpida
{

	class BenchmarkGroup final
	{
	public:

		[[nodiscard]] const std::vector<std::unique_ptr<Benchmark>>& getBenchmarks() const
		{
			return _benchmarks;
		}

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		BenchmarkGroup(std::string name, const std::vector<std::unique_ptr<Benchmark>>& benchmarks);
		~BenchmarkGroup() = default;
	private:
		std::string _name;
		const std::vector<std::unique_ptr<Benchmark>>& _benchmarks;
	};
}

#endif //APPS_QT_MODELS_BENCHMARKGROUP_HPP
