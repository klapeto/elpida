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

#include <Elpida/Config.hpp>

#include "ConfigurationViewsPool.hpp"

#include "Controllers/BenchmarksController.hpp"
#include "Controllers/BenchmarkRunnerController.hpp"
#include "Controllers/UploadController.hpp"

#include "Models/Benchmarks/BenchmarksModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"
#include "Models/BenchmarkConfigurationsCollectionModel.hpp"
#include "Models/BenchmarkRunnerModel.hpp"
#include "Models/SelectedBenchmarksModel.hpp"
#include "Models/GlobalConfigurationModel.hpp"
#include "Models/AffinityModel.hpp"
#include "UiModels/Screens/ScreensModel.hpp"

#include "Views/MainWindow/MainWindow.hpp"
#include "Views/SystemInfoView/SystemInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"
#include "Views/BenchmarkResultsView/BenchmarkResultsView.hpp"
#include "Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.hpp"
#include "Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.hpp"
#include "Views/BenchmarkListView/BenchmarkListView.hpp"
#include "Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp"
#include "Views/LogsView/LogsView.hpp"
#include "Views/QuickStartView/QuickStartView.hpp"

#include "Core/ElpidaMediator.hpp"
#include "Core/DataUploader.hpp"
#include "Core/JsonResultFormatter.hpp"

#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/OsInfo.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>
#include <Elpida/ServiceProvider.hpp>


#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QSplashScreen>
#include <QScreen>
#include "QCustomApplication.hpp"

#include <getopt.h>
#include <iostream>

#define NON_EXIT_CODE -1

using namespace Elpida;

void initializeTopologyTab(ScreensModel& screensModel, TopologyView& topologyWidget);
void loadDefaultGlobalConfiguration(GlobalConfigurationModel& configurationModel);
int processArgumentsAndCheckIfWeMustExit(GlobalConfigurationModel& configurationModel, int argC, char** argV);
void initializeTaskTab(ScreensModel& screensModel,
	BenchmarkListView& taskBatchesListWidget,
	BenchmarkResultsView& taskResultsWidget,
	BenchmarkRunnerStatusView& taskBatchRunnerStatusView,
	BenchmarkRunnerControlsView& taskBatchRunnerControlsView,
	BenchmarkConfigurationView& benchmarkConfigurationView);
void printHelp();
void printVersion();

#ifdef ELPIDA_LINUX
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void segFaultHandler(int sig)
{
	void* array[20];
	size_t size = backtrace(array, 20);

	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(EXIT_FAILURE);
}
#endif

static void setupPlatformSpecifics()
{
#ifdef ELPIDA_LINUX
	signal(SIGSEGV, segFaultHandler);
	signal(SIGABRT, segFaultHandler);
#endif
}

int main(int argc, char* argv[])
{
	setupPlatformSpecifics();

	GlobalConfigurationModel globalConfigurationModel;

	loadDefaultGlobalConfiguration(globalConfigurationModel);

	auto exitCode = processArgumentsAndCheckIfWeMustExit(globalConfigurationModel, argc, argv);
	if (exitCode != NON_EXIT_CODE)
	{
		return exitCode;
	}

	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QCoreApplication::setOrganizationName("Elpida");
	QCoreApplication::setOrganizationDomain(ELPIDA_WEBSITE_URL);
	QCoreApplication::setApplicationName("Elpida Qt");

	ElpidaMediator mediator;
	Logger logger;

	QCustomApplication application(argc, argv, mediator, logger);

	auto screenSize = QGuiApplication::primaryScreen()->size();
	auto pixmap = QIcon(":/Elpida_Splash_Screen.svg").pixmap(QSize(screenSize.width() / 2, screenSize.height() / 2));
	QSplashScreen splash(pixmap);

	splash.show();
	Elpida::QCustomApplication::processEvents();

	// System Information
	splash.showMessage("Getting CPU info...");
	const CpuInfo& cpuInfo = CpuInfo::get();

	splash.showMessage("Getting System topology...");
	SystemTopology topology;

	splash.showMessage("Getting Memory info...");
	MemoryInfo memoryInfo;

	splash.showMessage("Getting OS info...");
	OsInfo osInfo;

	splash.showMessage("Calculating overheads...");
	TimingInfo timingInfo;
	timingInfo.reCalculate(topology);

	ServiceProvider serviceProvider(cpuInfo, memoryInfo, topology, osInfo, timingInfo, logger);

	// UI
	splash.showMessage("Initializing Ui...");

	BenchmarkResultsModel taskRunResultsModel;
	JsonResultFormatter formatter(topology, cpuInfo, osInfo, memoryInfo, timingInfo);

	ScreensModel screensModel;
	AffinityModel affinityModel;
	MainWindow mainWindow(mediator, screensModel, affinityModel, taskRunResultsModel, formatter);
	ConfigurationViewsPool configurationViewsPool;
	QuickStartView quickStartView;

	screensModel.add(ScreenItem("Quick Start", quickStartView));

	SystemInfoView systemInfoView(cpuInfo, topology, osInfo, timingInfo, memoryInfo);
	screensModel.add(ScreenItem("System Info", systemInfoView));

	TopologyView topologyView(topology, affinityModel);

	initializeTopologyTab(screensModel, topologyView);

	BenchmarksModel taskBatchesModel;
	BenchmarkRunnerModel taskRunnerModel;
	BenchmarkConfigurationsCollectionModel benchmarkConfigurationsModel;
	SelectedBenchmarksModel selectedBenchmarksModel;
	BenchmarksController
		benchmarksController
		(taskBatchesModel, benchmarkConfigurationsModel, globalConfigurationModel, serviceProvider, logger);
	benchmarksController.reload();
	BenchmarkRunnerController
		runnerController
		(mediator,
			taskRunResultsModel,
			taskRunnerModel,
			benchmarkConfigurationsModel,
			selectedBenchmarksModel,
			affinityModel,
			serviceProvider,
			timingInfo,
			logger);
	BenchmarkListView taskBatchesListView(taskBatchesModel, selectedBenchmarksModel, mediator);
	BenchmarkResultsView benchmarkResultsView(taskRunResultsModel);
	BenchmarkRunnerStatusView benchmarkRunnerStatusView(taskRunnerModel);
	BenchmarkRunnerControlsView benchmarkRunnerControlsView(mediator, taskRunnerModel, globalConfigurationModel);
	BenchmarkConfigurationView benchmarkConfigurationView(benchmarkConfigurationsModel, configurationViewsPool, selectedBenchmarksModel);

	mediator.registerCommandHandler(runnerController);
	mediator.registerCommandHandler(mainWindow);

	initializeTaskTab(screensModel,
		taskBatchesListView,
		benchmarkResultsView,
		benchmarkRunnerStatusView,
		benchmarkRunnerControlsView,
		benchmarkConfigurationView);

	LogsView logsView(logger);
	screensModel.add(ScreenItem("Logs", logsView));

	UploadController uploadController(mediator, taskRunResultsModel, globalConfigurationModel, logger);
	mediator.registerCommandHandler(uploadController);

	DataUploader uploader(mediator, formatter, logger);
	mediator.registerCommandHandler(uploader);

	mainWindow.show();

	splash.finish(&mainWindow);

	return QApplication::exec();
}

void initializeTaskTab(ScreensModel& screensModel,
	BenchmarkListView& taskBatchesListWidget,
	BenchmarkResultsView& taskResultsWidget,
	BenchmarkRunnerStatusView& taskBatchRunnerStatusView,
	BenchmarkRunnerControlsView& taskBatchRunnerControlsView,
	BenchmarkConfigurationView& benchmarkConfigurationView)
{
	// TODO: create as normal widgets
	auto rootWidget = new QWidget();
	auto rootLayout = new QVBoxLayout();
	auto topLayout = new QHBoxLayout();

	topLayout->addWidget(&taskBatchesListWidget);
	topLayout->addWidget(&benchmarkConfigurationView);
	topLayout->addWidget(&taskResultsWidget);
	rootLayout->addLayout(topLayout);
	rootLayout->addWidget(&taskBatchRunnerStatusView);
	rootLayout->addWidget(&taskBatchRunnerControlsView);
	rootWidget->setLayout(rootLayout);

	screensModel.add(ScreenItem("Benchmarks", *rootWidget));
}

void initializeTopologyTab(ScreensModel& screensModel, TopologyView& topologyWidget)
{
	auto container = new QWidget;
	auto scrollArea = new QScrollArea;
	auto rootLayout = new QVBoxLayout;

	container->setLayout(rootLayout);
	container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	rootLayout->addWidget(scrollArea);

	scrollArea->setWidgetResizable(false);
	scrollArea->setWidget(&topologyWidget);
	scrollArea->setAlignment(Qt::AlignmentFlag::AlignCenter);

	topologyWidget.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	screensModel.add(ScreenItem("System Topology", *container));
}

void loadDefaultGlobalConfiguration(GlobalConfigurationModel& configurationModel)
{
	configurationModel.transactional<GlobalConfigurationModel>([](GlobalConfigurationModel& configurationModel)
	{
#if ELPIDA_DEBUG_BUILD
		configurationModel.setBenchmarksPath(TASK_BATCH_DEBUG_DIR);
#else
		configurationModel.setBenchmarksPath("./Benchmarks");
#endif
		configurationModel.setDataPath("./Data");
	});
}

int processArgumentsAndCheckIfWeMustExit(GlobalConfigurationModel& configurationModel, int argC, char* argV[])
{
	enum Opts
	{
		Arg_Version,
		Arg_Help,
		Arg_BenchmarkPath,
	};
	struct option options[] = {
		{ "version", no_argument, nullptr, 'v' },
		{ "help", no_argument, nullptr, 'h' },
		{ "benchmarksPath", required_argument, nullptr, 'b' },
		{ "dataPath", required_argument, nullptr, 'd' },
		{ nullptr, 0, nullptr, 0 }
	};

	int option_index = 0;
	int c = 0;
	while ((c = getopt_long(argC, argV, "v:h:b:d:", options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'v':
			printVersion();
			return EXIT_SUCCESS;
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
		case 'b':
			if (optarg != nullptr)
			{
				configurationModel.setBenchmarksPath(optarg);
			}
			else
			{
				std::cerr << "--benchmarksPath requires a path eg: --benchmarksPath=./Benchmarks";
				return EXIT_FAILURE;
			}
			break;
		case 'd':
			if (optarg != nullptr)
			{
				configurationModel.setDataPath(optarg);
			}
			else
			{
				std::cerr << "--dataPath requires a path eg: --dataPath=./Images";
				return EXIT_FAILURE;
			}
			break;
		case '?':
			return EXIT_FAILURE;
		default:
			break;
		}
	}
	return NON_EXIT_CODE;
}

void printVersion()
{
	std::cout << "Elpida Qt: " << ELPIDA_VERSION << std::endl;
	std::cout << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;
}

void printHelp()
{
	std::cout << "Elpida Qt: " << ELPIDA_VERSION << std::endl;
	std::cout << "Usage: " << std::endl;
	std::cout << "       -v, --version" << std::endl;
	std::cout << "           Prints the version and exits" << std::endl;
	std::cout << "       -h, --help" << std::endl;
	std::cout << "           Prints this help and exits" << std::endl;
	std::cout << "       -b=DIRECTORY, --benchmarksPath=DIRECTORY" << std::endl;
	std::cout << "           Overrides the directory to load the Benchmarks from" << std::endl;
	std::cout << "       -d=DIRECTORY, --dataPath=DIRECTORY" << std::endl;
	std::cout << "           Overrides the directory where the benchmarks will get their data" << std::endl;
}