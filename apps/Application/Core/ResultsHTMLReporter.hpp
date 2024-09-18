//
// Created by klapeto on 25/5/2024.
//

#ifndef ELPIDA_RESULTSHTMLREPORTER_HPP
#define ELPIDA_RESULTSHTMLREPORTER_HPP

#include <vector>
#include <filesystem>
#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{
	class BenchmarkRunConfigurationModel;
	class BenchmarkConfigurationModel;
	class BenchmarkStatisticsService;
	class PathsService;
	class FullBenchmarkResultModel;
	class CpuInfoModel;
	class OsInfoModel;

	class ResultsHTMLReporter
	{
	public:

		void WriteCustomBenchmarkReport(const std::vector<BenchmarkResultModel>& results,
				const std::vector<std::size_t>& affinity,
				const std::filesystem::path& outputFile) const;

		void WriteFullBenchmarkReport(const std::vector<FullBenchmarkResultModel>& results,
				Duration totalRunDuration,
				const std::filesystem::path& outputFile) const;

		explicit ResultsHTMLReporter(const BenchmarkRunConfigurationModel& runConfigurationModel,
				const CpuInfoModel& cpuInfoModel,
				const OsInfoModel& osInfoModel,
				const BenchmarkStatisticsService& statisticsService,
				const PathsService& pathsService);
	private:
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		const BenchmarkStatisticsService& _statisticsService;
		const PathsService& _pathsService;
		std::string _osInfo;
		std::string _cpuInfo;
		std::string _year;
	};

} // Application
// Elpida

#endif //ELPIDA_RESULTSHTMLREPORTER_HPP
