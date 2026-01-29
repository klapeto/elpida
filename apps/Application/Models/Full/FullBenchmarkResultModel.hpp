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

#ifndef ELPIDA_FULLBENCHMARKRESULTMODEL_HPP
#define ELPIDA_FULLBENCHMARKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Core/BenchmarkScore.hpp"

#include <vector>

namespace Elpida::Application
{

	class FullBenchmarkResultModel : public Model
	{
	public:

		const std::vector<BenchmarkResultModel>& GetBenchmarkResults() const;
		Score GetTotalScore() const;
		Score GetSingleThreadScore() const;
		Score GetMultiThreadScore() const;

		FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults, Score totalScore, Score singleThreadScore, Score multiThreadScore);

		FullBenchmarkResultModel(const FullBenchmarkResultModel&) = default;
		FullBenchmarkResultModel(FullBenchmarkResultModel&&) noexcept = default;
		FullBenchmarkResultModel& operator=(const FullBenchmarkResultModel&) = default;
		FullBenchmarkResultModel& operator=(FullBenchmarkResultModel&&) noexcept = default;
		~FullBenchmarkResultModel() override = default;
	private:
		std::vector<BenchmarkResultModel> _benchmarkResults;
		Score _totalScore;
		Score _singleThreadScore;
		Score _multiThreadScore;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKRESULTMODEL_HPP
