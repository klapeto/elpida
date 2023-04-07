#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QVBoxLayout>

#include <sstream>

#include "Elpida/Core/Config.hpp"

#include "Layouts/FlowLayout.hpp"
#include "Models/TopologyModel.hpp"

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

		_ui->statusbar->addWidget(new QLabel("Selected benchmarks:"), 0);
		_ui->statusbar->addWidget(_selectedBenchmarkLabel, 1);
		_ui->statusbar->addWidget(new QLabel("Selected cpus:"), 0);
		_ui->statusbar->addWidget(_selectedNodesLabel, 1);

		_topologyModelChanged = topologyModel.DataChanged().Subscribe([this]()
		{
			OnTopologyModelChanged();
		});
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
	}

	void MainWindow::OnTopologyModelChanged()
	{
		auto selected = _topologyModel.GetSelectedLeafNodes();
		if (selected.empty())
		{
			_selectedNodesLabel->setText(_nonSelected);
		}
		else
		{
			std::ostringstream accumulator;

			std::sort(selected.begin(), selected.end(), [](auto& a, auto& b){
				return a.get().GetOsIndex().value() < b.get().GetOsIndex().value();
			});

			const std::size_t maxShownElement = 8;
			if (selected.size() > maxShownElement)
			{
				auto maxSideElements = (maxShownElement / 2) - 1;
				accumulator << std::to_string(selected[0].get().GetOsIndex().value());

				for (std::size_t i = 1; i < maxSideElements; ++i)
				{
					accumulator << ", " << std::to_string(selected[i].get().GetOsIndex().value());
				}

				auto nextElement = selected.size() - maxSideElements;
				accumulator << " ... " << std::to_string(selected[nextElement].get().GetOsIndex().value());

				for (std::size_t i = nextElement + 1; i < selected.size(); ++i)
				{
					accumulator << ", " << std::to_string(selected[i].get().GetOsIndex().value());
				}
			}
			else
			{
				accumulator << std::to_string(selected[0].get().GetOsIndex().value());

				for (std::size_t i = 1; i < selected.size(); ++i)
				{
					accumulator << ", " << std::to_string(selected[i].get().GetOsIndex().value());
				}
			}
			_selectedNodesLabel->setText(QString::fromStdString(accumulator.str()));
		}
	}

}

