/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 30/9/2024.
//

#include "DefaultOutputFormatter.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include <sstream>

namespace Elpida::Application
{
	static std::string GetBenchmarkResult(const BenchmarkResultModel& resultModel)
	{
		std::ostringstream stream;

		auto& benchmark = resultModel.GetBenchmark();
		stream << resultModel.GetInstanceName()
			   << ": "
			   << ValueUtilities::GetValueScaleStringSI(resultModel.GetResult())
			   << benchmark.GetResultUnit()
			   << (benchmark.GetResultType() == ResultType::Throughput ? "/s" : "");

		return stream.str();
	}

	std::string DefaultOutputFormatter::FormatResults(const FullBenchmarkModel& model)
	{
		std::ostringstream stream;

		stream << "Elpida benchmark results: " << std::endl;
		std::size_t i = 0;
		for (auto& item : model.GetItems())
		{
			auto& result = item.GetValue();
			stream << "Iteration: " << i << std::endl;
			stream << "==============" << std::endl;
			stream << "\tTotal score: " << ValueUtilities::GetValueScaleStringSI(result.GetTotalScore()) << std::endl;
			stream << "\tSingle thread score: " << ValueUtilities::GetValueScaleStringSI(result.GetSingleThreadScore()) << std::endl;
			stream << "\tMulti thread score: " << ValueUtilities::GetValueScaleStringSI(result.GetMultiThreadScore()) << std::endl;
			stream << "\tBenchmark results:" << std::endl;
			for (auto& benchmarkResult : result.GetBenchmarkResults())
			{
				stream << '\t' << GetBenchmarkResult(benchmarkResult) << std::endl;
			}

			stream << std::endl;
		}

		return stream.str();
	}
} // application
// Elpida