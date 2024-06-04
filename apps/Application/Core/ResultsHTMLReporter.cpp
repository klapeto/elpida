//
// Created by klapeto on 25/5/2024.
//

#include "ResultsHTMLReporter.hpp"

#include <sstream>
#include <fstream>
#include <ctime>

#include "BenchmarkStatisticsService.hpp"
#include "PathsService.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	ResultsHTMLReporter::ResultsHTMLReporter(const BenchmarkRunConfigurationModel& runConfigurationModel,
			const BenchmarkStatisticsService& statisticsService, const PathsService& pathsService)
			:_runConfigurationModel(runConfigurationModel), _statisticsService(statisticsService),
			 _pathsService(pathsService)
	{
	}

	static std::string GetDateString()
	{
		auto time = std::time(nullptr);
		return std::asctime(std::localtime(&time));
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

	static std::string GenerateInfo(const BenchmarkModel& benchmarkModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel)
	{
		std::ostringstream stream;

		stream << "<table>";

		stream << "<tr><td>Benchmark</td><td><b>" << benchmarkModel.GetName() << "</b></td></tr>";
		stream << "<tr><td>Date</td><td>" << GetDateString() << "</td></tr>";
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

	static std::string GetScoreValue(double value, const std::string& unit)
	{
		return ValueUtilities::GetValueScaleStringSI(value) + unit;
	}

	static std::string GetNormalDelta(double value, double base)
	{
		std::ostringstream stream;

		stream << " <span>(";
		stream << ValueUtilities::ToFixed((value / base) * 100.0, 2);
		stream << "%)</span>";
		return stream.str();
	}

	static std::string GetScoreDelta(double value, double base)
	{
		std::ostringstream stream;

		stream << " <span";
		if (value < base)
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
		auto statistics = statisticsService.CalculateStatistics(results);
		auto& scoreUnit = results.front().GetBenchmark().GetScoreUnit();
		std::ostringstream stream;

		stream << "<table>";

		stream << "<tr><td>Sample size</td><td>" << ValueUtilities::GetValueScaleStringSI(statistics.sampleSize)
			   << "</td></tr>";
		stream << "<tr><td>Mean</td><td>" << GetScoreValue(statistics.mean, scoreUnit) << "</td></tr>";
		stream << "<tr><td>Max</td><td>" << GetScoreValue(statistics.max, scoreUnit)
			   << GetScoreDelta(statistics.max, statistics.mean) << "</td></tr>";
		stream << "<tr><td>Min</td><td>" << GetScoreValue(statistics.min, scoreUnit)
			   << GetScoreDelta(statistics.min, statistics.mean) << "</td></tr>";
		stream << "<tr><td>Std Dev</td><td>" << GetScoreValue(statistics.standardDeviation, scoreUnit)
			   << GetNormalDelta(statistics.standardDeviation, statistics.mean) << "</td></tr>";
		stream << "<tr><td>Margin of error</td><td>" << GetScoreValue(statistics.marginOfError, scoreUnit)
			   << GetNormalDelta(statistics.marginOfError, statistics.mean) << "</td></tr>";

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
				   << GetScoreValue(result.GetScore(), result.GetBenchmark().GetScoreUnit()) << "</td>";
			stream << "<td>";

			if (previousScore > -1.0)
			{
				stream << GetScoreDelta(result.GetScore(), previousScore);
			}

			stream << "</td></tr>";
			previousScore = result.GetScore();
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

	static std::string GetTemplateData(const PathsService& pathsService)
	{
		std::ifstream file(pathsService.GetExecutablePath() / "assets" / "custom-report-template.html",
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
		auto templateData = GetTemplateData(_pathsService);
		auto directory = outputFile;
		directory.remove_filename();
		create_directories(directory);

		auto index = templateData.find("<!--INFO-->");
		if (index == std::string::npos)
		{
			throw ElpidaException("Invalid HTML template. Missing '<!--INFO-->'");
		}
		templateData.insert(index, GenerateInfo(results.front().GetBenchmark(), _runConfigurationModel));

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

		std::ofstream outFile(outputFile);
		if (!outFile)
		{
			outFile.exceptions(std::ios_base::badbit);
		}

		outFile << templateData;
	}
} // Application
// Elpida