
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QVBoxLayout>

#include <sstream>
#include <filesystem>
#include <QMessageBox>
#include <QFileDialog>

#include "Elpida/Core/Config.hpp"

#include "Controllers/FullBenchmarkController.hpp"
#include "Controllers/MemoryBenchmarkController.hpp"
#include "Controllers/CustomBenchmarkController.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/Custom/CustomBenchmarkModel.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"

#include "Views/OsInfoView/OsInfoView.hpp"
#include "Views/MemoryInfoView/MemoryInfoView.hpp"
#include "Views/TimingInfoView/TimingInfoView.hpp"
#include "Views/CpuInfoView/CpuInfoView.hpp"
#include "Views/TopologyView/TopologyView.hpp"
#include "Views/CustomBenchmarkView/CustomBenchmarkView.hpp"
#include "Views/FullBenchmarkView/FullBenchmarkView.hpp"
#include "Views/MemoryBenchmarkView/MemoryBenchmarkView.hpp"

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

	MainWindow::MainWindow(
			const OsInfoModel& osInfoModel,
			const MemoryInfoModel& memoryInfoModel,
			const CpuInfoModel& cpuInfoModel,
			const TimingModel& timingModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			const FullBenchmarkModel& fullBenchmarkModel,
			const CustomBenchmarkModel& customBenchmarksModel,
			const MemoryBenchmarkModel& memoryOverheadCalculationModel,
			TopologyModel& topologyModel,
			FullBenchmarkController& fullBenchmarkController,
			CustomBenchmarkController& customBenchmarksController,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			MemoryBenchmarkController& memoryOverheadCalculationController,
			ConfigurationViewPool& configurationViewPool,
			QWidget* parent)
			:QMainWindow(parent),
			 _topologyModel(topologyModel),
			 _cpuInfoModel(cpuInfoModel),
			 _customBenchmarkController(customBenchmarksController),
			 _memoryBenchmarkController(memoryOverheadCalculationController),
			 _fullBenchmarkController(fullBenchmarkController),
			 _ui(new Ui::MainWindow)
	{
		_ui->setupUi(this);

		_ui->gbOsInfo->setLayout(new QVBoxLayout);
		_ui->gbOsInfo->layout()->addWidget(new OsInfoView(osInfoModel));

		_ui->gbMemoryInfo->setLayout(new QVBoxLayout);
		_ui->gbMemoryInfo->layout()->addWidget(new MemoryInfoView(memoryInfoModel));

		_ui->gbTimingInfo->setLayout(new QVBoxLayout);
		_ui->gbTimingInfo->layout()->addWidget(new TimingInfoView(timingModel));

		_ui->gbCpuInfo->setLayout(new QVBoxLayout);
		_ui->gbCpuInfo->layout()->addWidget(new CpuInfoView(cpuInfoModel));

		_ui->wTopologyContainer->setLayout(new QVBoxLayout);
		_ui->wTopologyContainer->layout()->addWidget(new TopologyView(topologyModel));

		_ui->tabFullBenchmark->layout()->addWidget(new FullBenchmarkView(benchmarkRunConfigurationModel,
				benchmarkRunConfigurationController, fullBenchmarkModel, fullBenchmarkController));

		_selectedNodesLabel = new QLabel(_nonSelected);

		_ui->statusbar->addWidget(new QLabel("Selected cpus:"), 0);
		_ui->statusbar->addWidget(_selectedNodesLabel, 1);

		_topologyModelChanged = topologyModel.DataChanged().Subscribe([this]()
		{
		  OnTopologyModelChanged();
		});

		_ui->tabMemoryBenchmark->layout()->addWidget(new MemoryBenchmarkView(memoryOverheadCalculationModel, memoryOverheadCalculationController, benchmarkRunConfigurationModel, benchmarkRunConfigurationController));

		_ui->tabCustomBenchmark->layout()->addWidget(
				new CustomBenchmarkView(customBenchmarksModel,
						benchmarkRunConfigurationModel, customBenchmarksController, benchmarkRunConfigurationController,
						configurationViewPool));
		OnTopologyModelChanged();
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

	void MainWindow::on_actionAbout_triggered()
	{
		QMessageBox::about(QApplication::activeWindow(), "About: Elpida", aboutText);
	}

	void MainWindow::on_actionClear_results_triggered()
	{
		auto index =_ui->tbBenchmark->currentIndex();
		switch (index)
		{
		case 1:
			_fullBenchmarkController.ClearResults();
			break;
		case 2:
			_memoryBenchmarkController.ClearResults();
			break;
		case 3:
			_customBenchmarkController.ClearResults();
			break;
		default:
			break;
		}
	}

	void MainWindow::on_actionSave_results_as_triggered()
	{
		auto filename = QFileDialog::getSaveFileName(this,
				tr("Save results"), "", tr("JSON File (*.json)"));
		auto index =_ui->tbBenchmark->currentIndex();
		switch (index)
		{
		case 1:
			_fullBenchmarkController.SaveResults(filename.toStdString());
			break;
		case 2:
			_memoryBenchmarkController.SaveResults(filename.toStdString());
			break;
		case 3:
			_customBenchmarkController.SaveResults(filename.toStdString());
			break;
		default:
			break;
		}
	}
}

