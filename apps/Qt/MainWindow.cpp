#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QVBoxLayout>

#include <sstream>
#include <QMessageBox>

#include "Elpida/Core/Config.hpp"

#include "Layouts/FlowLayout.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/BenchmarksModel.hpp"

#include "Views/OsInfoView/OsInfoView.hpp"
#include "Views/MemoryInfoView/MemoryInfoView.hpp"
#include "Views/OverheadsInfoView/OverheadsInfoView.hpp"
#include "Views/CpuInfoView/CpuInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"
#include "Views/BenchmarksView/BenchmarksView.hpp"

namespace Elpida::Application
{
	constexpr const char* aboutText = "<h2>Elpida " ELPIDA_VERSION "</h2>"
									  "<p>Elpida is an Open Source (GPLv3) Benchmarking framework for measuring "
									  "performance of computer hardware or algorithms.</p>"
									  "<p>It's goal is to be as transparent and open as possible "
									  "as well as extensible by the community. It comes with a library (libelpida), a Qt application and a "
									  "set of predefined benchmarks for measuring computer hardware capabilities.</p>"
									  "<p>Copyright (C) 2023  Ioannis Panagiotopoulos</p>"
									  "More info at: <a href=\"" ELPIDA_WEBSITE_URL "\">" ELPIDA_WEBSITE_URL "</a>";

	MainWindow::MainWindow(const OsInfoModel& osInfo,
		const MemoryInfoModel& memoryInfo,
		const CpuInfoModel& cpuInfo,
		const OverheadsModel& overheadsInfo,
		TopologyModel& topologyModel,
		BenchmarksModel& benchmarksModel,
		BenchmarksController& benchmarksController,
		ConfigurationViewPool& configurationViewPool,
		QWidget* parent)
		: QMainWindow(parent),
		_topologyModel(topologyModel),
		_benchmarksModel(benchmarksModel),
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

		_benchmarksModelChanged = benchmarksModel.DataChanged().Subscribe([this]()
		{
			OnBenchmarksModelChanged();
		});

		_ui->wBenchmarks->layout()->addWidget(new BenchmarksView(benchmarksModel, benchmarksController, configurationViewPool));
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

			std::sort(selected.begin(), selected.end(), [](auto& a, auto& b)
			{
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

	void MainWindow::on_actionExit_triggered()
	{
		QApplication::quit();
	}

	void MainWindow::on_actionSave_results_as_triggered()
	{
		//QMessageBox::about(QApplication::activeWindow(), "About: Elpida", "LOL");
	}

	void MainWindow::on_actionAbout_triggered()
	{
		QMessageBox::about(QApplication::activeWindow(), "About: Elpida", aboutText);
	}

	void MainWindow::OnBenchmarksModelChanged()
	{
		auto selectedBenchmark = _benchmarksModel.GetSelectedBenchmark();
		if (selectedBenchmark != nullptr)
		{
			_selectedBenchmarkLabel->setText(QString::fromStdString(selectedBenchmark->GetName()));
		}
		else
		{
			_selectedBenchmarkLabel->setText(_nonSelected);
		}
	}

}

