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
		const std::vector<BenchmarkResultModel>& GetBenchmarkResults() const;
		double GetTotalScore() const;
		double GetSingleCoreScore() const;
		double GetMultiCoreScore() const;
		double GetMemoryScore() const;

		FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults, double totalScore, double singleCoreScore, double multiCoreScore, double memoryScore);

		FullBenchmarkResultModel(const FullBenchmarkResultModel&) = delete;
		FullBenchmarkResultModel(FullBenchmarkResultModel&&) noexcept = default;
		FullBenchmarkResultModel& operator=(const FullBenchmarkResultModel&) = delete;
		FullBenchmarkResultModel& operator=(FullBenchmarkResultModel&&) noexcept = default;
		~FullBenchmarkResultModel() override = default;
	private:
		std::vector<BenchmarkResultModel> _benchmarkResults;
		double _totalScore;
		double _singleCoreScore;
		double _multiCoreScore;
		double _memoryScore;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKRESULTMODEL_HPP
