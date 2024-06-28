/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "Core/SettingsService.hpp"
#include "Elpida/Core/Config.hpp"

#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QScreen>
#include <QMessageBox>

#include <iostream>
#include <cctype>
#include <filesystem>
#include <string>

#include "MainWindow.hpp"
#include "ConfigurationViewPool.hpp"
#include "QtThreadQueue.hpp"
#include "QtSettingsService.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/Custom/CustomBenchmarkModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"
#include "Models/Full/FullBenchmarkModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/BenchmarkStatisticsService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Controllers/CustomBenchmarkController.hpp"
#include "Controllers/BenchmarkRunConfigurationController.hpp"
#include "Controllers/FullBenchmarkController.hpp"
#include "ModelBuilderJson.hpp"
#include "ResultSerializer.hpp"

#include "Elpida/Platform/Process.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "QtMessageService.hpp"
#include "QtPathsService.hpp"
#include "QtDesktopService.hpp"
#include "Controllers/MemoryBenchmarkController.hpp"

using namespace Elpida;
using namespace Elpida::Application;

#ifdef ELPIDA_UNIX

#include <execinfo.h>
#include <csignal>
#include <cstdlib>
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

static void SaveSelectedNodes(SettingsService& settingsService, TopologyModel& topologyModel)
{
	std::string selectedNodesStr;
	for (auto& node: topologyModel.GetSelectedLeafNodes())
	{
		selectedNodesStr += std::to_string(node.get().GetOsIndex().value()) + ',';
	}
	selectedNodesStr = selectedNodesStr.substr(0, selectedNodesStr.size() - 1);
	settingsService.Set("selectedNodes", selectedNodesStr);
}

static void LoadSelectedNodes(SettingsService& settingsService, TopologyModel& topologyModel)
{
	auto selectedNodes = settingsService.Get("selectedNodes");
	std::string buffer;
	auto leafs = topologyModel.GetLeafNodes();
	std::sort(leafs.begin(), leafs.end(), [](auto& a, auto& b)
	{ return a.get().GetOsIndex() < b.get().GetOsIndex(); });
	for (Size i = 0; i < selectedNodes.size(); ++i)
	{
		auto c = selectedNodes[i];
		if (c == ',')
		{
			if (buffer.empty()) continue;
			auto index = std::stoul(buffer);
			if (index < leafs.size())
			{
				leafs[index].get().SetSelected(true);
			}
			buffer.clear();
		}
		else if (std::isdigit(c))
		{
			buffer += c;
		}
	}

	if (buffer.empty()) return;
	auto index = std::stoul(buffer);
	if (index < leafs.size())
	{
		leafs[index].get().SetSelected(true);
	}
}

static std::string GetInfoData(const std::string& benchmarksPath)
{
	Process process(OsUtilities::GetExecutableDirectory() / "elpida-info-dumper", { benchmarksPath}, true, true);
	AsyncPipeReader stdOut(process.GetStdOut());
	AsyncPipeReader stdErr(process.GetStdErr());

	stdOut.StartReading();
	stdErr.StartReading();
	try
	{
		process.WaitToExit();
		stdOut.StopReading();
		return stdOut.GetString();
	}
	catch (...)
	{
		stdErr.StopReading();
		auto err = stdErr.GetString();
		if (err.empty())
		{
			throw;
		}
		else
		{
			throw ElpidaException("Info dumper process failed with error: ", err);
		}
	}
}

static void UpdateBenchmarkSettings(std::vector<BenchmarkGroupModel>& groups, SettingsService& settingsService)
{
	for (auto& group: groups)
	{
		for (auto& benchmark: group.GetBenchmarks())
		{
			for (auto& config: benchmark.GetConfigurations())
			{
				auto value = settingsService.Get(config.GetId());
				if (value.empty()) continue;
				config.SetValue(value);
			}
		}
	}
}


static ModelBuilderJson GetBasicInfo(const std::string& benchmarksPath){

	std::string data;
	std::string error;
	std::atomic<bool> done = false;
	auto th = std::thread([&]
	{
		try
		{
			data = GetInfoData(benchmarksPath);
		}
		catch (const std::exception& ex)
		{
			error = ex.what();
		}
		done.store(true, std::memory_order_release);
	});

	while (!done.load(std::memory_order_acquire))
	{
		QApplication::processEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	th.join();

	if (!error.empty())
	{
		QMessageBox::critical(QApplication::activeWindow(), "Critical error", QString::fromStdString(error));
		std::abort();
	}

	return ModelBuilderJson(data);
}

int main(int argc, char* argv[])
{
	setupPlatformSpecifics();

	try
	{
		ThreadQueue::SetCurrent(std::make_shared<QtThreadQueue>());

		QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
		QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

		QCoreApplication::setOrganizationName("Elpida");
		QCoreApplication::setOrganizationDomain(ELPIDA_WEBSITE_URL);
		QCoreApplication::setApplicationName("Elpida Qt");

		QApplication application(argc, argv);

		auto screenSize = QGuiApplication::primaryScreen()->size();
		auto pixmap = QIcon(":/Elpida_Splash_Screen.svg").pixmap(QSize(screenSize.width() / 2, screenSize.height() / 2));
		QSplashScreen splash(pixmap);

		splash.show();
		splash.showMessage("Getting system info (it should take about 5 seconds)...");
		QApplication::processEvents();

		ModelBuilderJson builderJson = GetBasicInfo(argc > 1 ? argv[1] : (OsUtilities::GetExecutableDirectory() / "Benchmarks").string());

		QtMessageService messageService;

		QtSettingsService settingsService;

		LoadSelectedNodes(settingsService, builderJson.GetTopologyInfoModel());

		auto benchmarkGroups = builderJson.GetBenchmarkGroups();

		UpdateBenchmarkSettings(benchmarkGroups, settingsService);

		BenchmarkExecutionService executionService;

		ResultSerializer resultSerializer(builderJson.GetCpuInfoModel(), builderJson.GetMemoryInfoModel(), builderJson.GetTopologyInfoModel(), builderJson.GetOsInfoModel(), builderJson.GetTimingModel());

		BenchmarkRunConfigurationModel benchmarkRunConfigurationModel;
		BenchmarkRunConfigurationController benchmarkRunConfigurationController(benchmarkRunConfigurationModel,
				settingsService);

		BenchmarkStatisticsService benchmarkStatisticsService;

		QtPathsService pathsService;
		QtDesktopService desktopService;

		ResultsHTMLReporter htmlReporter(benchmarkRunConfigurationModel, benchmarkStatisticsService, pathsService);

		CustomBenchmarkModel customBenchmarkModel(benchmarkGroups);
		CustomBenchmarkController
				customBenchmarkController(customBenchmarkModel, builderJson.GetTopologyInfoModel(),
				builderJson.GetTimingModel(),
				benchmarkRunConfigurationModel,
				executionService,
				resultSerializer,
				htmlReporter,
				pathsService,
				desktopService,
				messageService);

		ConfigurationViewPool configurationViewPool(settingsService);

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
				messageService,
				benchmarkGroups);

		MemoryBenchmarkModel memoryOverheadCalculationModel;
		MemoryBenchmarkController memoryOverheadCalculationController(memoryOverheadCalculationModel,
				builderJson.GetTimingModel(),
				builderJson.GetTopologyInfoModel(),
				builderJson.GetMemoryInfoModel(),
				benchmarkRunConfigurationModel,
				executionService,
				resultSerializer,
				htmlReporter,
				pathsService,
				desktopService,
				messageService,
				settingsService,
				benchmarkGroups);

		MainWindow mainWindow(
				builderJson.GetOsInfoModel(),
				builderJson.GetMemoryInfoModel(),
				builderJson.GetCpuInfoModel(),
				builderJson.GetTimingModel(),
				benchmarkRunConfigurationModel,
				fullBenchmarkModel,
				customBenchmarkModel,
				memoryOverheadCalculationModel,
				builderJson.GetTopologyInfoModel(),
				fullBenchmarkController,
				customBenchmarkController,
				benchmarkRunConfigurationController,
				memoryOverheadCalculationController,
				configurationViewPool);

		mainWindow.resize(QSize(screenSize.width() / 2, screenSize.height() / 2));
		mainWindow.show();

		splash.finish(&mainWindow);

		ThreadQueue::Current().lock()->Run();

		SaveSelectedNodes(settingsService, builderJson.GetTopologyInfoModel());
		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Failed to initialize: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
