/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include "Elpida/Config.hpp"

#include "Controllers/BenchmarksController.hpp"
#include "Controllers/BenchmarkRunnerController.hpp"

#include "Models/BenchmarksModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"
#include "Models/BenchmarkConfigurationsModel.hpp"

#include "Views/MainWindow/MainWindow.hpp"
#include "Views/SystemInfoView/SystemInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"
#include "Views/BenchmarkResultsView/BenchmarkResultsView.hpp"
#include "Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.hpp"
#include "Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.hpp"
#include "Views/BenchmarkListView/BenchmarkListView.hpp"
#include "Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp"

#include "Core/ElpidaMediator.hpp"

#include <Elpida/Topology/CpuInfo.hpp>
#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include "QCustomApplication.hpp"

#include <Elpida/Engine/Configuration/TaskConfiguration.hpp>
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>


using namespace Elpida;

#ifdef ELPIDA_LINUX
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void initializeTopologyTab(MainWindow& mainWindow, TopologyView& topologyWidget);

void initializeTaskTab(MainWindow& mainWindow,
	BenchmarkListView& taskBatchesListWidget,
	BenchmarkResultsView& taskResultsWidget,
	BenchmarkRunnerStatusView& taskBatchRunnerStatusView,
	BenchmarkRunnerControlsView& taskBatchRunnerControlsView,
	BenchmarkConfigurationView& benchmarkConfigurationView);
void segFaultHandler(int sig)
{
	void* array[20];
	size_t size = backtrace(array, 20);

	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
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

	ElpidaMediator mediator;

	QCustomApplication application(argc, argv, mediator);

	MainWindow mainWindow(mediator);

	CpuInfo cpuInfo;
	SystemTopology topology;

	SystemInfoView systemInfoView(cpuInfo, topology);
	mainWindow.addTab(systemInfoView, "System Info");

	TopologyView topologyView(topology);

	initializeTopologyTab(mainWindow, topologyView);

	Logger logger;
	BenchmarksModel taskBatchesModel;
	BenchmarkRunnerModel taskRunnerModel;
	BenchmarkResultsModel taskRunResultsModel;
	BenchmarkConfigurationsModel benchmarkConfigurationsModel;
	BenchmarksController taskBatchesController(taskBatchesModel, logger);
	taskBatchesController.reload();

	BenchmarkRunnerController runnerController(mediator, taskRunResultsModel, taskRunnerModel, benchmarkConfigurationsModel);
	mediator.registerCommandHandler(runnerController);


	BenchmarkListView taskBatchesListView(taskBatchesModel);
	BenchmarkResultsView benchmarkResultsView(taskRunResultsModel);
	BenchmarkRunnerStatusView benchmarkRunnerStatusView(taskRunnerModel);
	BenchmarkRunnerControlsView benchmarkRunnerControlsView(mediator, taskRunnerModel);
	BenchmarkConfigurationView benchmarkConfigurationView;

	mediator.registerCommandHandler(taskBatchesListView);
	mediator.registerCommandHandler(topologyView);
	mediator.registerCommandHandler(mainWindow);

	initializeTaskTab(mainWindow,
		taskBatchesListView,
		benchmarkResultsView,
		benchmarkRunnerStatusView,
		benchmarkRunnerControlsView,
		benchmarkConfigurationView);

	mainWindow.show();

	return QApplication::exec();
}

void initializeTaskTab(MainWindow& mainWindow,
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

	mainWindow.addTab(*rootWidget, "Task Batches");
}

void initializeTopologyTab(MainWindow& mainWindow, TopologyView& topologyWidget)
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

	mainWindow.addTab(*container, "System Topology");
}
