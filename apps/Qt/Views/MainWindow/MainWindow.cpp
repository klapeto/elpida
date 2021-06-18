/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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


#include "ui_MainWindow.h"
#include "MainWindow.hpp"

#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Core/JsonResultFormatter.hpp"
#include "UiModels/Screens/ScreensModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"
#include "Models/AffinityModel.hpp"
#include "Utilities/OsUtilities.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/SystemInfo/ProcessorNode.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>

#include <fstream>
#include <sstream>

namespace Elpida
{
	constexpr const char* aboutText = "<h2>Elpida " ELPIDA_VERSION "</h2>"
									  "<p>Elpida is an Open Source (GPLv3) Benchmarking framework for measuring "
									  "performance of computer hardware or algorithms.</p>"
									  "<p>It's goal is to be as transparent and open as possible "
									  "as well as extensible by the community. It comes with a library (libelpida), a Qt application and a "
									  "set of predefined benchmarks for measuring computer hardware capabilities.</p>"
									  "<p>Copyright (C) 2021  Ioannis Panagiotopoulos</p>"
									  "More info at: <a href=\"" ELPIDA_WEBSITE_URL "\">" ELPIDA_WEBSITE_URL "</a>";

	MainWindow::MainWindow(Mediator& mediator,
			ScreensModel& screensModel,
			const AffinityModel& affinityModel,
			BenchmarkResultsModel& benchmarkResultsModel,
			const ResultFormatter& resultFormatter,
			const CpuInfo& cpuInfo)
			: QMainWindow(),
			  _navigationBarView(screensModel),
			  _mediator(mediator),
			  _ui(new Ui::MainWindow),
			  _screensModel(screensModel),
			  _affinityModel(affinityModel),
			  _benchmarkResultsModel(benchmarkResultsModel),
			  _resultFormatter(resultFormatter),
			  _cpuInfo(cpuInfo)
	{
		_ui->setupUi(this);

		for (auto& itm: screensModel.getItems())
		{
			onScreenAdded(&itm.getValue());
		}

		addSubscription(_screensModel.itemAdded.subscribe([this](const CollectionChangedEventArgs<ScreenItem>& item)
		{
			emit screensModelItemAdded(&item.getItem().getValue());
		}));

		addSubscription(_screensModel.selectionChanged.subscribe([this](ScreenItem& item)
		{
			emit screensModelSelectedItemChanged(&item);
		}));

		addSubscription(_affinityModel.dataChanged.subscribe([this]
		{
			emit affinityChanged();
		}));

		QWidget::connect(this, &MainWindow::showMessageRequested, this, &MainWindow::showMessageRequestedHandler);
		QWidget::connect(this, &MainWindow::screensModelItemAdded, this, &MainWindow::onScreenAdded);
		QWidget::connect(this,
				&MainWindow::screensModelSelectedItemChanged, this, &MainWindow::onSelectedScreenChanged);

		QWidget::connect(this, &MainWindow::affinityChanged, this, &MainWindow::onAffinityChanged);

		_ui->wNavBar->layout()->addWidget(&_navigationBarView);

		_ui->statusBar
				->addPermanentWidget(new QLabel(
						QString("Elpida Version: " ELPIDA_VERSION " " ELPIDA_COMPILER_NAME " " ELPIDA_COMPILER_VERSION)));

		_ui->statusBar->addWidget(new QLabel("Selected processors:"));

		_processorsLabel = new QLabel();
		_processorsLabel->setWordWrap(true);
		_ui->statusBar->addWidget(_processorsLabel, 1);

		onAffinityChanged();
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
	}

	void MainWindow::on_actionExit_triggered()
	{
		QApplication::quit();
	}

	void MainWindow::on_actionAbout_triggered()
	{
		QMessageBox::about(QApplication::activeWindow(), "About: Elpida", aboutText);
	}

	void MainWindow::on_actionVisit_Website_triggered()
	{
		OsUtilities::openUrl(ELPIDA_WEBSITE_URL);
	}

	void MainWindow::handle(ShowMessageCommand& command)
	{
		emit showMessageRequested(QString::fromStdString(command.getMessage()), static_cast<int>(command.getType()));
	}

	void MainWindow::showMessageRequestedHandler(const QString& message, int type)
	{
		switch (static_cast<ShowMessageCommand::Type>(type))
		{
		case ShowMessageCommand::Type::Info:
			QMessageBox::information(this, "Information", message);
			break;
		case ShowMessageCommand::Type::Warning:
			QMessageBox::warning(this, "Warning", message);
			break;
		case ShowMessageCommand::Type::Error:
			QMessageBox::critical(this, "Error", message);
			break;
		}
	}

	void MainWindow::onScreenAdded(const ScreenItem* screen)
	{
		auto index = _ui->swPages->count();
		_ui->swPages->addWidget(screen->getWidget());
		_screensMaps.emplace(screen, index);
	}

	void MainWindow::onSelectedScreenChanged(const ScreenItem* screen)
	{
		auto index = _screensMaps.at(screen);
		_ui->swPages->setCurrentIndex(index);
	}

	void MainWindow::onAffinityChanged()
	{
		auto& processors = _affinityModel.getCurrentAffinity().getProcessorNodes();
		if (!processors.empty())
		{
			std::stringstream affinityStr;

			affinityStr << std::to_string(processors[0]->getOsIndex());

			for (size_t i = 1; i < processors.size(); ++i)
			{
				affinityStr << ',' << std::to_string(processors[i]->getOsIndex());
			}
			_processorsLabel->setText(QString::fromStdString(affinityStr.str()));
		}
		else
		{
			_processorsLabel
					->setText(
							"<strong><span style=\"color: #d73e3e\">No processors selected.</span> Please select from 'System Topology' tab before running benchmarks.</strong>");
		}
	}

	void MainWindow::on_actionSave_Results_As_triggered()
	{
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptSave);

		auto filter = QString::fromStdString("Elpida results file (*." + _resultFormatter.getFileExtension() + ")");
		dialog.setNameFilter(filter);
		dialog.selectNameFilter(filter);

		auto targetFilename = _cpuInfo.getModelName() + "-" + std::to_string(time(nullptr)) + ".json";
		dialog.selectFile(QString::fromStdString(targetFilename));

		if (dialog.exec())
		{
			QStringList fileNames = dialog.selectedFiles();
			const auto& filepath = fileNames.at(0);

			std::fstream fp(filepath.toStdString(), std::fstream::trunc | std::fstream::out);

			fp.exceptions();

			std::vector<BenchmarkResult> results;
			for (auto& result: _benchmarkResultsModel.getItems())
			{
				results.push_back(result.getValue());
			}

			fp << _resultFormatter.serialize(results);

			fp.close();

			QMessageBox::information(this, "Save successful",
					"Save was successful. Results were saved in: " + filepath);
		}
	}

	void MainWindow::on_actionClear_results_triggered()
	{
		_benchmarkResultsModel.clear();
	}

}  // namespace Elpida
