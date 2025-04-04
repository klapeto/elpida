#include "CustomBenchmarkView.hpp"
#include "ui_CustomBenchmarkView.h"

#include "Models/Custom/CustomBenchmarkModel.hpp"
#include "Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Controllers/CustomBenchmarkController.hpp"

#include "Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.hpp"
#include "Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.hpp"
#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"
#include <exception>
#include <qmessagebox.h>

#include <thread>
#include <atomic>

namespace Elpida::Application
{
	CustomBenchmarkView::CustomBenchmarkView(const CustomBenchmarkModel& benchmarksModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			CustomBenchmarkController& benchmarksController,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			ConfigurationViewPool& configurationViewPool)
		: QWidget(),
		  _ui(new Ui::CustomBenchmarkView),
		  _customBenchmarksModel(benchmarksModel),
		  _customBenchmarksController(benchmarksController),
		  _uiUpdating(false)
	{
		_ui->setupUi(this);

		LoadBenchmarkTree();

		_configurationView = new CustomBenchmarkConfigurationView(benchmarksModel, configurationViewPool);

		_ui->glMain->addWidget(_configurationView, 0, 1, 2, 1);

		_resultsView = new CustomBenchmarkResultsView(benchmarksModel);
		_ui->glMain->addWidget(_resultsView, 0, 2, 2,1);

		_runConfigurationView = new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel, benchmarkRunConfigurationController);
		_runConfigurationView->DisableUpload();

		static_cast<QVBoxLayout*>(_ui->gbExecution->layout())->insertWidget(0, _runConfigurationView);
		UpdateUi();
	}

	void CustomBenchmarkView::LoadBenchmarkTree()
	{
		for (auto& group : _customBenchmarksModel.GetBenchmarkGroups())
		{
			auto groupItem = new QTreeWidgetItem(QStringList(QString::fromStdString(group.GetName())));

			for (auto& benchmark : group.GetBenchmarks())
			{
				auto key = group.GetName() + benchmark.GetName();
				_benchmarkMap.insert({ key, const_cast<BenchmarkModel*>(&benchmark) });
				auto item = new QTreeWidgetItem(QStringList(QString::fromStdString(benchmark.GetName())));
				item->setData(1, 0, QVariant(QString::fromStdString(key)));
				groupItem->addChild(item);
			}

			_ui->twBenchmarks->addTopLevelItem(groupItem);
		}
	}

	CustomBenchmarkView::~CustomBenchmarkView()
	{
		delete _ui;
	}

	void CustomBenchmarkView::on_twBenchmarks_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
	{
		BenchmarkModel* benchmarkModel = nullptr;
		if (current != nullptr)
		{
			auto key = current->data(1, 0).value<QString>();
			auto itr = _benchmarkMap.find(key.toStdString());

			if (itr != _benchmarkMap.end())
			{
				benchmarkModel = itr->second;
			}
		}
		_customBenchmarksController.SetCurrentBenchmark(benchmarkModel);
		UpdateUi();
	}

	void CustomBenchmarkView::UpdateUi()
	{
		if (_uiUpdating) return;
		_uiUpdating = true;
		_ui->pbRun->setEnabled(_customBenchmarksModel.GetSelectedBenchmark() != nullptr);
		_uiUpdating = false;
	}

	void CustomBenchmarkView::on_pbRun_clicked(bool checked)
	{
		Run();
	}

	void CustomBenchmarkView::Run()
	{
		BenchmarkRunningDialog dialog(this);

		std::string exceptionMessage;
		std::atomic<bool> rejected = false;
		auto th = std::thread([this, &dialog, &exceptionMessage, &rejected]()
		{
		  try
		  {
			  _customBenchmarksController.Run();
		  }
		  catch (const std::exception& ex)
		  {
			  exceptionMessage = ex.what();
		  }

		  while (!rejected.load(std::memory_order_acquire) && !dialog.isVisible())
		  {
			  std::this_thread::sleep_for(MilliSeconds(10));
		  }
		  if (!rejected.load(std::memory_order_acquire))
		  {
			  dialog.accept();
		  }
		});
		if (dialog.exec() == QDialog::Rejected)
		{
			rejected.store(true, std::memory_order_release);
			_customBenchmarksController.StopRunning();
		}
		th.join();

		if (!exceptionMessage.empty())
		{
			QMessageBox::critical(this, "Error", QString::fromStdString(exceptionMessage));
			return;
		}

		UpdateUi();
	}
}