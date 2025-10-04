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
// Created by klapeto on 12/3/2023.
//

#include "Elpida/EntryPoint/DefaultFormatter.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/BenchmarkInfo.hpp"

#include <sstream>

namespace Elpida
{
	std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
	{
		return ValueUtilities::GetValueScaleStringSI(result.GetResult()) + benchmarkInfo.GetResultUnit() + (benchmarkInfo.GetResultType() == ResultType::Throughput ? "/s" : "");
	}

	String DefaultFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		std::ostringstream accumulator;

		auto benchmarkInfo = benchmark.GetInfo();
		accumulator << "Score: " << TranslateResult(result, benchmarkInfo) << std::endl;
		return accumulator.str();
	}
} // Elpida