//
// Created by klapeto on 8/4/20.
//

#ifndef ELPIDAMEDIATOR_HPP
#define ELPIDAMEDIATOR_HPP

#include <Elpida/Topology/SystemTopology.hpp>
#include <Elpida/Topology/CpuInfo.hpp>
#include <Elpida/MemoryInfo.hpp>
#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Utilities/Logger.hpp>
#include <sstream>
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QtWidgets/QApplication>
#include "Ui/MainWindow/MainWindow.hpp"

#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{
	class QtTaskBatchWrapper;

	class ElpidaMediator final : public Mediator
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

		std::unordered_map<std::string, QtTaskBatchWrapper*> _createdTaskBatches;
		std::stringstream _log;

		friend class ElpidaCommandHandler;
	};
}


#endif //ELPIDAMEDIATOR_HPP
