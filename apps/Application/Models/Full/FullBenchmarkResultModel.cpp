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
// Created by klapeto on 21/10/2023.
//

#include "FullBenchmarkResultModel.hpp"


namespace Elpida::Application
{
	FullBenchmarkResultModel::FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults,
			Score totalScore,
			Score singleThreadScore,
			Score multiThreadScore)
			:_benchmarkResults(std::move(benchmarkResults)),
			 _totalScore(totalScore),
			 _singleThreadScore(singleThreadScore),
			 _multiThreadScore(multiThreadScore)
	{

	}

	const std::vector<BenchmarkResultModel>& FullBenchmarkResultModel::GetBenchmarkResults() const
	{
		return _benchmarkResults;
	}

	Score FullBenchmarkResultModel::GetSingleThreadScore() const
	{
		return _singleThreadScore;
	}

	Score FullBenchmarkResultModel::GetMultiThreadScore() const
	{
		return _multiThreadScore;
	}

	Score FullBenchmarkResultModel::GetTotalScore() const
	{
		return _totalScore;
	}
} // Elpida
// Application