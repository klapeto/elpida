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

#include "Controllers/CoreController.hpp"
#include "Controllers/TaskBatchesController.hpp"
#include "Controllers/TaskRunnerController.hpp"

#include "Models/TaskBatchesModel.hpp"
#include "Models/TaskRunResultsModel.hpp"

#include "Ui/MainWindow/MainWindow.hpp"
#include "Ui/SystemInfoWidget/SystemInfoWidget.hpp"
#include "Ui/TopologyWidget/TopologyWidget.hpp"
#include "Ui/TaskResultsWidget/TaskResultsWidget.hpp"
#include "Ui/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.hpp"
#include "Ui/TaskBatchRunnerControlsView/TaskBatchRunnerControlsView.hpp"
#include "Ui/TaskBatchesListWidget/TaskBatchesListWidget.hpp"
#include "Ui/TaskResultsWidget/TaskResultsWidget.hpp"

#include "Core/ElpidaMediator.hpp"

#include <Elpida/Topology/CpuInfo.hpp>
#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>


using namespace Elpida;

#ifdef ELPIDA_LINUX
#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void initializeTopologyTab(MainWindow& mainWindow, TopologyWidget& topologyWidget);

void initializeTaskTab(MainWindow& mainWindow,
	TaskBatchesListWidget& taskBatchesListWidget,
	TaskResultsWidget& taskResultsWidget,
	TaskBatchRunnerStatusView& taskBatchRunnerStatusView,
	TaskBatchRunnerControlsView& taskBatchRunnerControlsView);
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
#endif
}

int main(int argc, char* argv[])
{
	setupPlatformSpecifics();

	ElpidaMediator mediator;

	CoreController coreController(argc, argv);
	mediator.registerCommandHandler(coreController);

	MainWindow mainWindow(mediator);

	CpuInfo cpuInfo;
	SystemTopology topology;

	SystemInfoWidget systemInfoWidget(cpuInfo, topology);
	mainWindow.addTab(systemInfoWidget, "System Info");

	TopologyWidget topologyWidget(topology);

	initializeTopologyTab(mainWindow, topologyWidget);

	Logger logger;
	TaskBatchesModel taskBatchesModel;
	TaskRunnerModel taskRunnerModel;
	TaskRunResultsModel taskRunResultsModel;
	TaskBatchesController taskBatchesController(taskBatchesModel, logger);
	taskBatchesController.reload();

	TaskRunnerController runnerController(mediator, taskRunResultsModel, taskRunnerModel);
	mediator.registerCommandHandler(runnerController);


	TaskBatchesListWidget taskBatchesListWidget(taskBatchesModel);
	TaskResultsWidget taskResultsWidget(taskRunResultsModel);
	TaskBatchRunnerStatusView taskBatchRunnerStatusView(taskRunnerModel);
	TaskBatchRunnerControlsView taskBatchRunnerControlsView(mediator, taskRunnerModel);

	mediator.registerCommandHandler(taskBatchesListWidget);
	mediator.registerCommandHandler(topologyWidget);

	initializeTaskTab(mainWindow,
		taskBatchesListWidget,
		taskResultsWidget,
		taskBatchRunnerStatusView,
		taskBatchRunnerControlsView);

	mainWindow.show();

	coreController.run();
	return 0;
}

void initializeTaskTab(MainWindow& mainWindow,
	TaskBatchesListWidget& taskBatchesListWidget,
	TaskResultsWidget& taskResultsWidget,
	TaskBatchRunnerStatusView& taskBatchRunnerStatusView,
	TaskBatchRunnerControlsView& taskBatchRunnerControlsView)
{
	// TODO: create as normal widgets
	auto rootWidget = new QWidget();
	auto rootLayout = new QVBoxLayout();
	auto topLayout = new QHBoxLayout();

	topLayout->addWidget(&taskBatchesListWidget);
	topLayout->addWidget(&taskResultsWidget);
	rootLayout->addLayout(topLayout);
	rootLayout->addWidget(&taskBatchRunnerStatusView);
	rootLayout->addWidget(&taskBatchRunnerControlsView);
	rootWidget->setLayout(rootLayout);

	mainWindow.addTab(*rootWidget, "Task Batches");
}

void initializeTopologyTab(MainWindow& mainWindow, TopologyWidget& topologyWidget)
{
	auto container = new QWidget;
	auto scrollArea = new QScrollArea;
	auto rootLayout = new QVBoxLayout;

	container->setLayout(rootLayout);
	container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	rootLayout->addWidget(scrollArea);

	scrollArea->setWidgetResizable(false);
	scrollArea->setWidget(&topologyWidget);

	topologyWidget.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	mainWindow.addTab(*container, "System Topology");
}
