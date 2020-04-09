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
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QtWidgets/QApplication>

#include "Ui/LogsDialog/LogsDialog.hpp"
#include "Ui/MainWindow/MainWindow.hpp"
#include "Ui/SystemInfoWidget/SystemInfoWidget.hpp"

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
		~ElpidaMediator();
	private:
		SharedLibraryLoader _batchLoader;
		SystemTopology _topology;
		CpuInfo _cpuInfo;
		MemoryInfo _memoryInfo;
		Logger _logger;

		QApplication _qApplication;

		MainWindow _mainWindow;
		SystemInfoWidget _systemInfoWidget;
		LogsDialog _logsDialog;

		std::unordered_map<std::string, QtTaskBatchWrapper*> _createdTaskBatches;

	private:
		void handle(const Command& command) override;
		void handle(const ShowLogsDialogCommand& command) override;
		void handle(const ShowAboutDialogCommand& command) override;
		void handle(const ExitApplicationCommand& command) override;
	};
}


#endif //APPS_QT_CORE_ELPIDAMEDIATOR_HPP
