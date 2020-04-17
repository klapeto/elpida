//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ELPIDAMEDIATOR_HPP
#define APPS_QT_CORE_ELPIDAMEDIATOR_HPP

#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Topology/CpuInfo.hpp>
#include <Elpida/MemoryInfo.hpp>
#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>

#include <QtWidgets/QApplication>

#include "Models/TaskRunnerModel.hpp"
#include "Models/TaskRunResultsModel.hpp"
#include "Models/TaskBatchesModel.hpp"

#include "Controllers/TaskBatchesController.hpp"

#include "Ui/LogsWidget/LogsWidget.hpp"
#include "Ui/MainWindow/MainWindow.hpp"
#include "Ui/SystemInfoWidget/SystemInfoWidget.hpp"
#include "Ui/TopologyWidget/TopologyWidget.hpp"
#include "Ui/TaskBatchesListWidget/TaskBatchesListWidget.hpp"
#include "Ui/TaskResultsWidget/TaskResultsWidget.hpp"
#include "Ui/CommonDialog/CommonDialog.hpp"
#include "Ui/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.hpp"
#include "Ui/TaskBatchRunnerControlsView/TaskBatchRunnerControlsView.hpp"

#include "Core/Abstractions/Mediator.hpp"
#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class QtTaskBatchWrapper;

	class ElpidaMediator final : public Mediator, private CommandHandler
	{
	public:
		void execute(const Command& command) override;
		void run();

		ElpidaMediator(int& argC, char** argv);
		~ElpidaMediator() override = default;
	private:
		QApplication _qApplication;

		SystemTopology _topology;
		CpuInfo _cpuInfo;
		Logger _logger;

		MainWindow _mainWindow;
		SystemInfoWidget _systemInfoWidget;
		LogsWidget _logsWidget;
		TopologyWidget _topologyWidget;

		TaskRunResultsModel _runResultsModel;
		TaskResultsWidget _taskResultsWidget;

		CommonDialog _commonDialog;

		TaskBatchesModel _taskBatchesModel;
		TaskBatchesController _taskBatchesController;
		TaskBatchesListWidget _taskBatchesListWidget;

		TaskRunnerModel _taskRunnerModel;
		TaskBatchRunnerStatusView _runnerStatusView;
		TaskBatchRunnerControlsView _taskBatchRunnerControlsView;
	private:
		void handle(const Command& command) override;
		void handle(const ShowLogsDialogCommand& command) override;
		void handle(const ShowAboutDialogCommand& command) override;
		void handle(const ExitApplicationCommand& command) override;
		void initializeSystemTopologyWidget();
		void initializeTaskBatchesTab();
	};
}


#endif //APPS_QT_CORE_ELPIDAMEDIATOR_HPP
