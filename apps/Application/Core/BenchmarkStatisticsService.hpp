//
// Created by klapeto on 26/5/2024.
//

#ifndef ELPIDA_BENCHMARKSTATISTICSSERVICE_HPP
#define ELPIDA_BENCHMARKSTATISTICSSERVICE_HPP

#include <cstddef>
#include <vector>

#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{
	struct BenchmarkStatistics
	{
		std::size_t sampleSize;
		double mean;
		double min;
		double max;
		double marginOfError;
		double standardDeviation;
	};

	class BenchmarkStatisticsService
	{
	public:

		[[nodiscard]]
		BenchmarkStatistics CalculateStatistics(const std::vector<BenchmarkResultModel>& results) const;
		BenchmarkStatisticsService() = default;
	};

} // Application
// Elpida

#endif //ELPIDA_BENCHMARKSTATISTICSSERVICE_HPP
