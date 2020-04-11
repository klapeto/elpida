//
// Created by klapeto on 8/4/20.
//

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QHBoxLayout>
#include <Elpida/Utilities/FileSystem.hpp>
#include <Elpida/ElpidaException.hpp>
#include "ElpidaMediator.hpp"
#include "Core/Abstractions/Command.hpp"

namespace Elpida
{

	ElpidaMediator::ElpidaMediator(int& argC, char** argv)
		: _qApplication(argC, argv), _mainWindow(*this), _systemInfoWidget(*this, _cpuInfo, _topology),
		  _logsDialog(&_mainWindow, _logger), _topologyWidget(_topology)
	{
		_mainWindow.addTab(&_systemInfoWidget, "System Info");
		initializeSystemTopologyWidget();
#if ELPIDA_DEBUG_BUILD
		_taskBatchPath = TASK_BATCH_DEBUG_DIR;
#else
		_taskBatchPath = "./TaskBatches";	// TODO: Think of something more portable
#endif

		loadTaskBatches();
	}
	void ElpidaMediator::initializeSystemTopologyWidget()
	{
		auto container = new QWidget;
		auto scrollArea = new QScrollArea;
		auto rootLayout = new QVBoxLayout;

		container->setLayout(rootLayout);
		container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

		rootLayout->addWidget(scrollArea);

		scrollArea->setWidgetResizable(false);
		scrollArea->setWidget(&_topologyWidget);

		_topologyWidget.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

		_mainWindow.addTab(container, "System Topology");
	}

	void ElpidaMediator::execute(const Command& command)
	{
		command.accept(*this);
	}

	void ElpidaMediator::run()
	{
		_mainWindow.show();
		QApplication::exec();
	}

	void ElpidaMediator::handle(const Command& command)
	{

	}

	void ElpidaMediator::handle(const ShowLogsDialogCommand& command)
	{
		_logsDialog.exec();
	}

	void ElpidaMediator::handle(const ShowAboutDialogCommand& command)
	{
		QMessageBox::about(
			&_mainWindow,
			"About: Elpida",
			"Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://github.com/klapeto/elpida");
	}

	void ElpidaMediator::handle(const ExitApplicationCommand& command)
	{
		QApplication::quit();
	}

	static bool hasLibraryExtension(const std::string& path)
	{
		if (!path.empty())
		{
			auto& ext = SharedLibraryLoader::LibrariesExtension;
			if (!ext.empty())
			{
				const auto pSize = path.size();
				const auto eSize = ext.size();

				if (pSize <= eSize) return false;    // path is less than extension or it is the extension

				for (auto pi = pSize - 1, ei = eSize - 1; ei > 0; --pi, --ei)
				{
					if (ext[ei] == path[pi]) continue;
					return false;
				}
				return true;
			}
			else
			{
				// wtf?
				throw ElpidaException(__func__, "Library extension has no value. Probably a corrupt build or bug");
			}
		}
		return false;
	}

	void ElpidaMediator::loadTaskBatches()
	{
		try
		{
			FileSystem::iterateDirectory(_taskBatchPath, [this](const std::string& path)
			{
				if (hasLibraryExtension(path))
				{
					try
					{
						_batchLoader.load(path);
					}
					catch (const std::exception& ex)
					{
						_logger.log(LogType::Error, "Failed to load Library:'" + path + "'", ex);
					}
				}
			});
		}
		catch (const std::exception& ex)
		{
			_logger.log(LogType::Error, "Failed to iterate Directory:'" + _taskBatchPath + "'", ex);
		}
	}

}