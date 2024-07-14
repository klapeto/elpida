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
