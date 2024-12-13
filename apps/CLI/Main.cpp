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
// Created by klapeto on 29/9/2024.
//

#include "ArgumentsHelper.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"
#include "Models/Full/FullBenchmarkModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/BenchmarkStatisticsService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Controllers/BenchmarkRunConfigurationController.hpp"
#include "Controllers/FullBenchmarkController.hpp"
#include "ModelBuilderJson.hpp"
#include "ResultSerializer.hpp"
#include "DataUploader.hpp"

#include "Elpida/Platform/OsUtilities.hpp"
#include "InfoGetter.hpp"
#include "CliSettingsService.hpp"
#include "CliPathsService.hpp"
#include "CliMessageService.hpp"
#include "CliDesktopService.hpp"
#include "DefaultOutputFormatter.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <fstream>

#ifdef ELPIDA_UNIX

#include <execinfo.h>
#include <csignal>
#include <unistd.h>

void segFaultHandler(int sig)
{
	void* array[20];
	int size = backtrace(array, 20);

	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

#endif

static void setupPlatformSpecifics()
{
#ifdef ELPIDA_UNIX
	signal(SIGSEGV, segFaultHandler);
	signal(SIGABRT, segFaultHandler);
#endif
}

#define ELPIDA_OUT(x) if (!helper.IsQuiet()) x

using namespace Elpida;
using namespace Elpida::Application;

template<typename TCallable>
static void WriteResultsToFile(TCallable callable, const std::filesystem::path& filePath)
{
	std::fstream file(filePath, std::ios::out | std::ios::trunc);
	file << callable();
}

int main(int argC, char** argV)
{
	setupPlatformSpecifics();

	try
	{
		OsUtilities::ConvertArgumentsToUTF8(argC, argV);

		std::locale::global(std::locale::classic());
		ArgumentsHelper helper;

		{
			std::string returnText;
			auto success = helper.ParseAndGetExitText(argC, argV, returnText);
			if (!returnText.empty())
			{
				std::cout << returnText << std::endl;
				return success ? EXIT_SUCCESS : EXIT_FAILURE;
			}
		}

		std::filesystem::path benchmarksPath;
		if (!helper.GetBenchmarksPath().empty())
		{
			std::string pathString = helper.GetBenchmarksPath().string();
			ValueUtilities::DeQuoteString(pathString);
			benchmarksPath = pathString;
		}
		else
		{
			benchmarksPath = OsUtilities::GetExecutableDirectory() / "Benchmarks";
		}

		ELPIDA_OUT(std::cout << "Getting System information..." << std::endl);

		ModelBuilderJson builderJson = ModelBuilderJson(InfoGetter::GetInfoData(benchmarksPath));

		BenchmarkExecutionService executionService;

		ResultSerializer resultSerializer(builderJson.GetCpuInfoModel(), builderJson.GetMemoryInfoModel(),
				builderJson.GetTopologyInfoModel(), builderJson.GetOsInfoModel(), builderJson.GetTimingModel());

		auto benchmarkGroups = builderJson.GetBenchmarkGroups();

		CliSettingsService settingsService;
		BenchmarkRunConfigurationModel benchmarkRunConfigurationModel;
		BenchmarkRunConfigurationController benchmarkRunConfigurationController(benchmarkRunConfigurationModel,
				settingsService);

		benchmarkRunConfigurationModel.SetDelaySecondsBetweenRuns(helper.GetSecondsBetweenRuns());
		benchmarkRunConfigurationModel.SetIterationsToRun(helper.GetIterationsToRun());
		benchmarkRunConfigurationModel.SetGenerateHtmlReport(helper.IsGenerateReport());
		benchmarkRunConfigurationModel.SetUploadResults(helper.IsUpload());

		BenchmarkStatisticsService benchmarkStatisticsService;

		CliPathsService pathsService(helper.GetReportPath());

		ResultsHTMLReporter htmlReporter(benchmarkRunConfigurationModel, builderJson.GetCpuInfoModel(),
				builderJson.GetOsInfoModel(), benchmarkStatisticsService, pathsService);

		DataUploader uploader(resultSerializer);

		CliMessageService messageService(helper.IsQuiet());

		CliDesktopService desktopService(helper.IsQuiet());

		FullBenchmarkModel fullBenchmarkModel;
		FullBenchmarkController fullBenchmarkController(fullBenchmarkModel,
				builderJson.GetTimingModel(),
				builderJson.GetTopologyInfoModel(),
				builderJson.GetMemoryInfoModel(),
				benchmarkRunConfigurationModel,
				executionService,
				resultSerializer,
				htmlReporter,
				pathsService,
				desktopService,
				uploader,
				messageService,
				benchmarkGroups);

		if (!fullBenchmarkController.GetMissingBenchmarks().empty())
		{
			std::ostringstream message;
			message << "The following benchmarks are missing: ";
			for (auto& benchmark : fullBenchmarkController.GetMissingBenchmarks())
			{
				message << benchmark << ", ";
			}
			message << ". Upload is disabled.";
			std::cerr << message.str() << std::endl;
		}

		if (!builderJson.GetFailedToLoadBenchmarkGroups().empty())
		{
			for (auto& [file, reason] : builderJson.GetFailedToLoadBenchmarkGroups())
			{
				std::cerr << "Failed to load benchmark group File: " << file << std::endl << "Reason:" << reason;
			}
		}

		std::size_t i = 0;
		std::size_t total = fullBenchmarkController.GetBenchmarks().size() * helper.GetIterationsToRun();
		EventSubscription<const std::string&> progressSubscription;
		if (!helper.IsQuiet())
		{
			progressSubscription = fullBenchmarkModel.CurrentRunningBenchmarkChanged().Subscribe([&](auto& name)
			{
				std::cout << "Currently executing: " << name << "\t(Progress: " << i++ << "/" << total << ")"
						  << std::endl;
			});
		}

		ELPIDA_OUT(std::cout << "Starting benchmarking. Estimated execution time: " << std::setprecision(2)
					  << ValueUtilities::GetTimeScaleValue(Seconds(5.0 * total)) << std::endl);

		fullBenchmarkController.RunAsync();
		fullBenchmarkController.WaitToCompleteRun();

		auto& resultPath = helper.GetResultPath();

		auto writeResults = [&]()
		{
			if (helper.GetOutputFormat() == OutputFormat::Default)
			{
				return Elpida::Application::DefaultOutputFormatter::FormatResults(fullBenchmarkModel);
			}
			else
			{
				std::vector<FullBenchmarkResultModel> results;
				results.reserve(fullBenchmarkModel.Size());

				for (auto& item : fullBenchmarkModel.GetItems())
				{
					results.push_back(item.GetValue());
				}
				return resultSerializer.Serialize(results);
			}
		};

		if (resultPath.empty())
		{
			std::cout << writeResults();
		}
		else
		{
			WriteResultsToFile(writeResults, resultPath);
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}