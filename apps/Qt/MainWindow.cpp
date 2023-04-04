#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QVBoxLayout>

#include "Layouts/FlowLayout.hpp"

#include "Views/OsInfoView/OsInfoView.hpp"
#include "Views/MemoryInfoView/MemoryInfoView.hpp"
#include "Views/OverheadsInfoView/OverheadsInfoView.hpp"
#include "Views/CpuInfoView/CpuInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"

namespace Elpida::Application
{
	MainWindow::MainWindow(const OsInfoModel& osInfo,
			const MemoryInfoModel& memoryInfo,
			const CpuInfoModel& cpuInfo,
			const OverheadsModel& overheadsInfo,
			TopologyModel& topologyModel,
			QWidget* parent) :
			QMainWindow(parent),
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
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
	}

}

