//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 25/5/2024.
//

#include "ResultsHTMLReporter.hpp"

#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>

#include "BenchmarkStatisticsService.hpp"
#include "PathsService.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/Benchmark/TaskModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"
#include "Models/Full/FullBenchmarkResultModel.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"
#include "Models/SystemInfo/OsInfoModel.hpp"

namespace Elpida::Application
{
	static const char* GetDeviationClass(double deviationPercent)
	{
		if (deviationPercent > 1.0) return "negative";
		if (deviationPercent > 0.5) return "neutral";
		return "positive";
	}

	static std::string GenerateCpuInfo(const CpuInfoModel& cpuInfoModel)
	{
		std::ostringstream stream;

		stream << "<table>";

		stream << "<tr><td>Architecture</td><td><b>" << cpuInfoModel.GetArchitecture() << "</b></td></tr>";
		stream << "<tr><td>Vendor</td><td><b>" << cpuInfoModel.GetVendorName() << "</b></td></tr>";
		stream << "<tr><td>Model</td><td><b>" << cpuInfoModel.GetModelName() << "</b></td></tr>";

		stream << "</table>";

		return stream.str();
	}

	static std::string GenerateOsInfo(const OsInfoModel& osInfoModel)
	{
		std::ostringstream stream;

		stream << "<table>";

		stream << "<tr><td>Category</td><td><b>" << osInfoModel.GetCategory() << "</b></td></tr>";
		stream << "<tr><td>Name</td><td><b>" << osInfoModel.GetName() << "</b></td></tr>";
		stream << "<tr><td>Version</td><td><b>" << osInfoModel.GetVersion() << "</b></td></tr>";

		stream << "</table>";

		return stream.str();
	}

	static std::string GetYear()
	{
		auto time = std::time(nullptr);
		return std::to_string(std::localtime(&time)->tm_year + 1900);
	}

	ResultsHTMLReporter::ResultsHTMLReporter(const BenchmarkRunConfigurationModel& runConfigurationModel,
			const CpuInfoModel& cpuInfoModel,
			const OsInfoModel& osInfoModel,
			const BenchmarkStatisticsService& statisticsService,
			const PathsService& pathsService)
			:_runConfigurationModel(runConfigurationModel), _statisticsService(statisticsService),
			 _pathsService(pathsService)
	{
		_cpuInfo = GenerateCpuInfo(cpuInfoModel);
		_osInfo = GenerateOsInfo(osInfoModel);
		_year = GetYear();
	}

	static std::string GetDateString()
	{
		auto time = std::time(nullptr);
		auto tm =std::localtime(&time);
		std::ostringstream stream;

		stream << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
		return stream.str();
	}

	static const char* GetConcurrencyModeStr(ConcurrencyMode concurrencyMode)
	{
		switch (concurrencyMode)
		{
		case ConcurrencyMode::None:
			return "None";
		case ConcurrencyMode::CopyInput:
			return "Copy input";
		case ConcurrencyMode::ShareInput:
			return "Share input";
		case ConcurrencyMode::ChunkInput:
			return "Chunk input";
		}
		return "Unknown";
	}

	static std::string GenerateCustomInfo(const BenchmarkModel& benchmarkModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel)
	{
		std::ostringstream stream;

		auto date = GetDateString();

		stream << "<table>";

		stream << "<tr><td>Benchmark</td><td><b>" << benchmarkModel.GetName() << "</b></td></tr>";
		stream << "<tr><td>Date</td><td><time datetime='" << date << "'>" << date << "</time></td></tr>";
		stream << "<tr><td>Concurrency hint</td><td>"
			   << GetConcurrencyModeStr(runConfigurationModel.GetConcurrencyMode()) << "</td></tr>";
		stream << "<tr><td><label for='numa-aware'>Numa aware</label></td><td><input id='numa-threads' type='checkbox' "
			   << (runConfigurationModel.IsNumaAware() ? "checked" : "") << "/></td></tr>";
		stream
				<< "<tr><td><label for='pined-threads'>Pined threads</label></td><td><input id='pined-threads' type='checkbox' "
				<< (runConfigurationModel.IsPinThreads() ? "checked" : "") << "/></td></tr>";

		stream << "</table>";

		return stream.str();
	}

	static std::string GenerateExecutionInfo(Duration duration)
	{
		std::ostringstream stream;

		stream << "<table>";

		auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
		auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) - hours;
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) - hours - minutes;

		auto date = GetDateString();

		stream << "<tr><td>Date generated</td><td><time datetime='" << date << "'>" << date << "</time></td></tr>";
		stream << "<tr><td>Run duration</td><td><time datetime='" << hours.count() << "h " << minutes.count() << "m " << seconds.count() <<"s'>"
			   << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s"
			   << "</time></td></tr>";

		stream << "</table>";

		return stream.str();
	}

	static std::string GetBenchmarkScoreValue(double value, const std::string& unit, ResultType resultType)
	{
		std::ostringstream stream;

		stream << "<data value='" << value << "' title='" << value << "'>";
		stream << ValueUtilities::GetValueScaleStringSI(value) << unit << (resultType == ResultType::Throughput ? "/s" : "");
		stream << "</data>";
		return stream.str();
	}

	static std::string GetScoreValue(double value)
	{
		std::ostringstream stream;

		stream << "<data value='" << value << "' title='" << value << "'>";
		stream << ValueUtilities::ToFixed(value, 2);
		stream << "</data>";
		return stream.str();
	}

	static std::string GetNormalDelta(double value, double base)
	{
		std::ostringstream stream;

		stream << " <span>(";
		stream << ValueUtilities::ToFixed((value / base) * 100.0, 2);
		stream << "%)</span>";
		return stream.str();
	}

	static std::string GetDeviationDelta(double value, double base)
	{
		std::ostringstream stream;

		auto percent = (value / base) * 100.0;

		stream << " <span class='" << GetDeviationClass(percent) << "'>(";
		stream << ValueUtilities::ToFixed(percent, 2);
		stream << "%)</span>";
		return stream.str();
	}

	static std::string GetScoreDelta(double value, double base, bool lessIsBetter)
	{
		std::ostringstream stream;

		auto isBetter = [&]()
		{ return lessIsBetter ? value > base : value < base; };

		stream << " <span";
		if (isBetter())
		{
			stream << " class='negative'>(-";
			stream << ValueUtilities::ToFixed(100 - ((value / base) * 100.0), 2);
		}
		else
		{
			stream << " class='positive'>(+";
			stream << ValueUtilities::ToFixed(100 - ((base / value) * 100.0), 2);
		}
		stream << "%)</span>";
		return stream.str();
	}

	static std::string GenerateStatistics(const std::vector<BenchmarkResultModel>& results,
			const BenchmarkStatisticsService& statisticsService)
	{
		auto statistics = statisticsService.CalculateStatistics(results, [](auto& r)
		{ return r.GetResult(); });

		auto& benchmark =results.front().GetBenchmark();
		auto& scoreUnit = benchmark.GetResultUnit();

		auto resultType = benchmark.GetResultType();
		auto lessIsBetter = resultType != ResultType::Throughput;

		std::ostringstream stream;

		stream << "<table><tbody>";

		stream << "<tr><td>Sample size</td><td>" << ValueUtilities::GetValueScaleStringSI(statistics.sampleSize, 0)
			   << "</td></tr>";
		stream << "<tr><td>Mean</td><td><b>" << GetBenchmarkScoreValue(statistics.mean, scoreUnit, resultType) << "</b></td></tr>";
		stream << "<tr><td>Max</td><td>" << GetBenchmarkScoreValue(statistics.max, scoreUnit, resultType)
			   << GetScoreDelta(statistics.max, statistics.mean, lessIsBetter) << "</td></tr>";
		stream << "<tr><td>Min</td><td>" << GetBenchmarkScoreValue(statistics.min, scoreUnit, resultType)
			   << GetScoreDelta(statistics.min, statistics.mean, lessIsBetter) << "</td></tr>";
		stream << "<tr><td>Std Dev</td><td>" << GetBenchmarkScoreValue(statistics.standardDeviation, scoreUnit,
				resultType)
			   << GetDeviationDelta(statistics.standardDeviation, statistics.mean) << "</td></tr>";
		stream << "<tr><td>Margin of error</td><td>" << GetBenchmarkScoreValue(statistics.marginOfError, scoreUnit,
				resultType)
			   << GetNormalDelta(statistics.marginOfError, statistics.mean) << "</td></tr>";

		stream << "</tbody></table>";

		return stream.str();
	}

	static std::string GenerateAllBenchmarkResults(const std::vector<FullBenchmarkResultModel>& results)
	{
		std::ostringstream stream;

		for (std::size_t i = 0; i < results.size(); ++i)
		{
			auto& result = results[i];
			stream << "<h3>Iteration #" << i + 1
				   << " Single: " << GetScoreValue(result.GetSingleThreadScore())
				   << " Multi: " << GetScoreValue(result.GetMultiThreadScore())
				   << " Total: " << GetScoreValue(result.GetTotalScore())
				   << "</h3>";

			stream << "<table><thead><tr><th>Name</th><th>Score</th><th>Delta</th></tr></thead>";
			stream << "<tbody>";

			auto& benchmarkResults = result.GetBenchmarkResults();
			for (std::size_t j = 0; j < benchmarkResults.size(); ++j)
			{
				auto& benchmarkResult = benchmarkResults[j];
				auto& benchmark = benchmarkResult.GetBenchmark();
				stream << "<tr>"
					   << "<td>" << benchmarkResult.GetInstanceName() << "</td>"
					   << "<td>" << GetBenchmarkScoreValue(benchmarkResult.GetResult(), benchmark.GetResultUnit(),
						benchmark.GetResultType()) << "</td>";

				if (i > 0)
				{
					auto& previousResult = results[i - 1].GetBenchmarkResults()[j];
					stream << "<td>"
						   << GetScoreDelta(benchmarkResult.GetResult(), previousResult.GetResult(),
								   benchmark.GetResultType() != ResultType::Throughput)
						   << "</td>";
				}
				else
				{
					stream << "<td></td>";
				}

				stream << "</tr>";
			}

			stream << "</tbody></table>";

			if (i + 1 < results.size())
			{
				stream << "<hr>";
			}
		}

		return stream.str();
	}

	static std::string GenerateBenchmarkStatistics(const std::vector<FullBenchmarkResultModel>& results,
			const BenchmarkStatisticsService& statisticsService)
	{

		std::ostringstream stream;

		stream << "<table>"
				  "<thead>"
				  "<tr>"
				  "<th>Name</th>"
				  "<th>Mean</th>"
				  "<th>Max</th>"
				  "<th>Min</th>"
				  "<th>Std Dev</th>"
				  "<th>Margin of error</th>"
				  "</tr>"
				  "</thead>";

		if (!results.empty())
		{
			auto totalBenchmarks = results.front().GetBenchmarkResults().size();

			for (std::size_t i = 0; i < totalBenchmarks; ++i)
			{
				auto statistics = statisticsService.CalculateStatistics(results,
						[i](auto& r)
						{ return r.GetBenchmarkResults()[i].GetResult(); });

				auto& benchmarkResult = results.front().GetBenchmarkResults()[i];
				auto& benchmark = benchmarkResult.GetBenchmark();
				auto& resultUnit = benchmark.GetResultUnit();
				auto resultType = benchmark.GetResultType();

				stream << "<tr>"
					   << "<td>" << benchmarkResult.GetInstanceName() << "</td>"
					   << "<td><b>" << GetBenchmarkScoreValue(statistics.mean, resultUnit, resultType) << "</b></td>"
					   << "<td>" << GetBenchmarkScoreValue(statistics.max, resultUnit, resultType) << "</td>"
					   << "<td>" << GetBenchmarkScoreValue(statistics.min, resultUnit, resultType) << "</td>"
					   << "<td>" << GetBenchmarkScoreValue(statistics.standardDeviation, resultUnit, resultType)
					   << GetDeviationDelta(statistics.standardDeviation, statistics.mean) << "</span>"
					   << "</td>"
					   << "<td>" << GetBenchmarkScoreValue(statistics.marginOfError, resultUnit, resultType)
					   << GetNormalDelta(statistics.marginOfError, statistics.mean) << "</td>"
					   << "</tr>";
			}
		}


		stream << "</table>";

		return stream.str();
	}

	static std::string GenerateConfiguration(const std::vector<BenchmarkResultModel>& results)
	{
		if (results.empty()) return "";
		std::ostringstream stream;

		auto& result = results[0];

		stream << "<table><thead><tr><th>Name</th><th>Value</th></tr></thead>";

		for (auto& config : result.GetConfiguration())
		{
			stream << "<tr>"
				   << "<td>" << config.first << "</td>" << "<td>" << config.second << "</td>"
				   << "</tr>";
		}

		stream << "</table>";

		return stream.str();
	}

	static std::string GenerateResults(const std::vector<BenchmarkResultModel>& results)
	{
		if (results.empty()) return "";
		std::ostringstream stream;

		stream << "<table><thead><tr><th>Iteration</th><th>Score</th><th>Diff from previous</th></tr></thead>";

		double previousScore = -1.0;
		for (std::size_t i = 0; i < results.size(); ++i)
		{
			auto& result = results[i];
			stream << "<tr>"
				   << "<td>" << i + 1 << "</td>" << "<td>"
				   << GetBenchmarkScoreValue(result.GetResult(), result.GetBenchmark().GetResultUnit(),
						   result.GetBenchmark().GetResultType()) << "</td>";
			stream << "<td>";

			if (previousScore > -1.0)
			{
				stream << GetScoreDelta(result.GetResult(), previousScore,
						result.GetBenchmark().GetResultType() != ResultType::Throughput);
			}

			stream << "</td></tr>";
			previousScore = result.GetResult();
		}

		stream << "</table>";

		return stream.str();
	}

	template<typename TCallable>
	static std::string GenerateFullStatistic(const std::vector<FullBenchmarkResultModel>& results,
			const std::string& name, const BenchmarkStatisticsService& statisticsService, TCallable getter)
	{
		auto statistics = statisticsService.CalculateStatistics(results, getter);
		std::ostringstream stream;

		stream << "<table><caption>";
		stream << name;
		stream << "</caption><tbody><tr><td>Sample size</td><td>"
			   << ValueUtilities::GetValueScaleStringSI(statistics.sampleSize, 0)
			   << "</td></tr>";
		stream << "<tr><td>Mean</td><td><b>" << GetBenchmarkScoreValue(statistics.mean, "", ResultType::Custom) << "</b></td></tr>";
		stream << "<tr><td>Max</td><td>" << GetBenchmarkScoreValue(statistics.max, "", ResultType::Custom)
			   << GetScoreDelta(statistics.max, statistics.mean, false) << "</td></tr>";
		stream << "<tr><td>Min</td><td>" << GetBenchmarkScoreValue(statistics.min, "", ResultType::Custom)
			   << GetScoreDelta(statistics.min, statistics.mean, false) << "</td></tr>";
		stream << "<tr><td>Std Dev</td><td>" << GetBenchmarkScoreValue(statistics.standardDeviation, "",
				ResultType::Custom)
			   << GetDeviationDelta(statistics.standardDeviation, statistics.mean) << "</td></tr>";
		stream << "<tr><td>Margin of error</td><td>" << GetBenchmarkScoreValue(statistics.marginOfError, "",
				ResultType::Custom)
			   << GetNormalDelta(statistics.marginOfError, statistics.mean) << "</td></tr>";

		stream << "</tbody></table>";

		return stream.str();
	}

	static std::string GenerateFullStatistics(const std::vector<FullBenchmarkResultModel>& results,
			const BenchmarkStatisticsService& statisticsService)
	{
		std::ostringstream stream;

		stream << GenerateFullStatistic(results, "Single core", statisticsService, [](auto& r)
		{ return r.GetSingleThreadScore(); });
		stream << GenerateFullStatistic(results, "Multi core", statisticsService, [](auto& r)
		{ return r.GetMultiThreadScore(); });
		stream << GenerateFullStatistic(results, "Total core", statisticsService, [](auto& r)
		{ return r.GetTotalScore(); });

		return stream.str();
	}

	static std::string GenerateFullResults(const std::vector<FullBenchmarkResultModel>& results)
	{
		if (results.empty()) return "";
		std::ostringstream stream;

		stream
				<< "<table><thead><tr><th>Iteration</th><th>Single core score</th><th>Multi core score</th><th>Total score</th></tr></thead>";

		Score previousSingleScore = -1.0;
		Score previousMultiScore = -1.0;
		Score previousTotalScore = -1.0;
		for (std::size_t i = 0; i < results.size(); ++i)
		{
			auto& result = results[i];

			if (previousSingleScore > -1.0)
			{
				stream << "<tr>"
					   << "<td>" << i + 1 << "</td>"
					   << "<td>" << GetScoreValue(result.GetSingleThreadScore())
					   << GetScoreDelta(result.GetSingleThreadScore(), previousSingleScore, false) << "</td>"
					   << "<td>" << GetScoreValue(result.GetMultiThreadScore())
					   << GetScoreDelta(result.GetMultiThreadScore(), previousMultiScore, false) << "</td>"
					   << "<td>" << GetScoreValue(result.GetTotalScore())
					   << GetScoreDelta(result.GetTotalScore(), previousTotalScore, false) << "</td></tr>";
			}
			else
			{
				stream << "<tr>"
					   << "<td>" << i + 1 << "</td>"
					   << "<td>" << GetScoreValue(result.GetSingleThreadScore()) << "</td>"
					   << "<td>" << GetScoreValue(result.GetMultiThreadScore()) << "</td>"
					   << "<td>" << GetScoreValue(result.GetTotalScore()) << "</td></tr>";
			}

			previousSingleScore = result.GetSingleThreadScore();
			previousMultiScore = result.GetMultiThreadScore();
			previousTotalScore = result.GetTotalScore();
		}

		stream << "</table>";

		return stream.str();
	}

	static std::string GenerateAffinity(const std::vector<std::size_t>& affinity)
	{
		std::ostringstream stream;

		stream << "<pre>";

		if (!affinity.empty())
		{
			for (auto& processor : affinity)
			{
				stream << processor << ", ";
			}
			stream.seekp(-2, std::ostringstream::end);    //remove the last ", "
		}

		stream << "</pre>";

		return stream.str();
	}

	static std::string GetTemplateData(const std::string& filename, const PathsService& pathsService)
	{
		std::ifstream file(pathsService.GetExecutablePath() / "assets" / filename,
				std::ios::binary);
		if (!file.is_open())
		{
			file.exceptions(std::ifstream::failbit);
		}
		file.seekg(0, std::ios_base::end);
		auto size = file.tellg();
		file.seekg(0);

		std::string str;
		str.resize(size);
		file.read(str.data(), size);
		return str;
	}

	void ResultsHTMLReporter::WriteCustomBenchmarkReport(const std::vector<BenchmarkResultModel>& results,
			const std::vector<std::size_t>& affinity,
			const std::filesystem::path& outputFile) const
	{
		auto templateData = GetTemplateData("custom-report-template.html", _pathsService);
		auto directory = outputFile;
		directory.remove_filename();
		create_directories(directory);

		auto index = templateData.find("<!--INFO-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--INFO-->'");
		}
		templateData.insert(index, GenerateCustomInfo(results.front().GetBenchmark(), _runConfigurationModel));

		index = templateData.find("<!--STATISTICS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--STATISTICS-->'");
		}

		templateData.insert(index, GenerateStatistics(results, _statisticsService));

		index = templateData.find("<!--CONFIG-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--CONFIG-->'");
		}

		templateData.insert(index, GenerateConfiguration(results));

		index = templateData.find("<!--AFFINITY-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--AFFINITY-->'");
		}

		templateData.insert(index, GenerateAffinity(affinity));


		index = templateData.find("<!--RESULTS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--RESULTS-->'");
		}

		templateData.insert(index, GenerateResults(results));

		index = templateData.find("<!--RESULTS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--RESULTS-->'");
		}

		templateData.insert(index, GenerateResults(results));

		std::ofstream outFile(outputFile);
		if (!outFile)
		{
			outFile.exceptions(std::ios_base::badbit);
		}

		outFile << templateData;
	}

	void ResultsHTMLReporter::WriteFullBenchmarkReport(const std::vector<FullBenchmarkResultModel>& results,
			Duration totalRunDuration,
			const std::filesystem::path& outputFile) const
	{
		auto templateData = GetTemplateData("full-report-template.html", _pathsService);
		auto directory = outputFile;
		directory.remove_filename();
		create_directories(directory);

		auto index = templateData.find("<!--EXECUTION-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--INFO-->'");
		}

		templateData.insert(index, GenerateExecutionInfo(totalRunDuration));

		index = templateData.find("<!--CPU-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--CPU-->'");
		}

		templateData.insert(index, _cpuInfo);

		index = templateData.find("<!--OS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--OS-->'");
		}

		templateData.insert(index, _osInfo);

		index = templateData.find("<!--STATISTICS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--STATISTICS-->'");
		}

		templateData.insert(index, GenerateFullStatistics(results, _statisticsService));

		index = templateData.find("<!--RESULTS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--RESULTS-->'");
		}

		templateData.insert(index, GenerateFullResults(results));

		index = templateData.find("<!--BENCHMARK_STATISTICS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--BENCHMARK_STATISTICS-->'");
		}

		templateData.insert(index, GenerateBenchmarkStatistics(results, _statisticsService));


		index = templateData.find("<!--ALL_BENCHMARK_RESULTS-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--ALL_BENCHMARK_RESULTS-->'");
		}

		templateData.insert(index, GenerateAllBenchmarkResults(results));


		index = templateData.find("<!--YEAR-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--YEAR-->'");
		}

		templateData.insert(index, _year);

		std::ofstream outFile(outputFile);
		if (!outFile)
		{
			outFile.exceptions(std::ios_base::badbit);
		}

		outFile << templateData;
	}
} // Application
// Elpida