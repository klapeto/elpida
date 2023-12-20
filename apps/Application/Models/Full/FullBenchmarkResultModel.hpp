//
// Created by klapeto on 21/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKRESULTMODEL_HPP
#define ELPIDA_FULLBENCHMARKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"

#include <vector>

namespace Elpida::Application
{

	class FullBenchmarkResultModel : public Model
	{
	public:
		using Score = double;

		const std::vector<BenchmarkResultModel>& GetBenchmarkResults() const;
		Score GetTotalScore() const;
		Score GetSingleCoreScore() const;
		Score GetMultiCoreScore() const;
		Score GetMemoryScore() const;

		FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults, Score totalScore, Score singleCoreScore, Score multiCoreScore, Score memoryScore);

		FullBenchmarkResultModel(const FullBenchmarkResultModel&) = delete;
		FullBenchmarkResultModel(FullBenchmarkResultModel&&) noexcept = default;
		FullBenchmarkResultModel& operator=(const FullBenchmarkResultModel&) = delete;
		FullBenchmarkResultModel& operator=(FullBenchmarkResultModel&&) noexcept = default;
		~FullBenchmarkResultModel() override = default;
	private:
		std::vector<BenchmarkResultModel> _benchmarkResults;
		Score _totalScore;
		Score _singleCoreScore;
		Score _multiCoreScore;
		Score _memoryScore;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKRESULTMODEL_HPP