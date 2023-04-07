#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QVBoxLayout>

#include <sstream>

#include "Layouts/FlowLayout.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/TopologyNodeModel.hpp"

#include "Views/OsInfoView/OsInfoView.hpp"
#include "Views/MemoryInfoView/MemoryInfoView.hpp"
#include "Views/OverheadsInfoView/OverheadsInfoView.hpp"
#include "Views/CpuInfoView/CpuInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"

#include "Elpida/Core/ValueUtilities.hpp"

namespace Elpida::Application
{
	MainWindow::MainWindow(const OsInfoModel& osInfo,
			const MemoryInfoModel& memoryInfo,
			const CpuInfoModel& cpuInfo,
			const OverheadsModel& overheadsInfo,
			TopologyModel& topologyModel,
			QWidget* parent) :
			QMainWindow(parent),
			_topologyModel(topologyModel),
			_ui(new Ui::MainWindow)
	{
		_ui->setupUi(this);

		_ui->gbOsInfo->setLayout(new QVBoxLayout);
		_ui->gbOsInfo->layout()->addWidget(new OsInfoView(osInfo));

		_ui->gbMemoryInfo->setLayout(new QVBoxLayout);
		_ui->gbMemoryInfo->layout()->addWidget(new MemoryInfoView(memoryInfo));

		_ui->gbOverheadsInfo->setLayout(new QVBoxLayout);
		_ui->gbOverheadsInfo->layout()->addWidget(new OverheadsInfoView(overheadsInfo));

		_ui->gbCpuInfo->setLayout(new QVBoxLayout);
		_ui->gbCpuInfo->layout()->addWidget(new CpuInfoView(cpuInfo));

		_ui->wTopologyContainer->setLayout(new QVBoxLayout);
		_ui->wTopologyContainer->layout()->addWidget(new TopologyView(topologyModel));

		_selectedNodesLabel = new QLabel(_nonSelected);
		_selectedBenchmarkLabel = new QLabel(_nonSelected);
		_ui->statusbar->addWidget(new QLabel("Selected benchmarks:"));
		_ui->statusbar->addWidget(_selectedBenchmarkLabel);
		_ui->statusbar->addWidget(new QLabel("Selected cpus:"));
		_ui->statusbar->addWidget(_selectedNodesLabel);

		_topologyModelChanged = topologyModel.DataChanged().Subscribe([this]()
		{
			auto selected = _topologyModel.GetSelectedLeafNodes();
			if (selected.empty())
			{
				_selectedNodesLabel->setText(_nonSelected);
			}
			else
			{
				std::ostringstream accumulator;

				for (auto& node: selected)
				{
					accumulator
					<< node.get().GetOsIndex().value()
					<< ',';
				}
				_selectedNodesLabel->setText(QString::fromStdString(accumulator.str()));
			}
		});
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
	}

}

