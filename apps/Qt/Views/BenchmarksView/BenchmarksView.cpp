#include "BenchmarksView.hpp"
#include "Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.hpp"
#include "Elpida/Core/Duration.hpp"
#include "ui_BenchmarksView.h"

#include "Models/BenchmarksModel.hpp"

#include "Controllers/BenchmarksController.hpp"

#include "Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp"
#include "Views/BenchmarkResultsView/BenchmarkResultsView.hpp"
#include <exception>
#include <qmessagebox.h>

#include <thread>
#include <atomic>

namespace Elpida::Application
{
	BenchmarksView::BenchmarksView(
		const BenchmarksModel& benchmarksModel,
		const BenchmarkResultsModel& benchmarkResultsModel,
		BenchmarksController& benchmarksController,
		ConfigurationViewPool& configurationViewPool)
		: QWidget(),
		  _ui(new Ui::BenchmarksView),
		  _benchmarksModel(benchmarksModel),
		  _benchmarksController(benchmarksController),
		  _uiUpdating(false)
	{
		_ui->setupUi(this);

		LoadBenchmarkTree();

		_configurationView = new BenchmarkConfigurationView(benchmarksModel, configurationViewPool);

		_ui->glMain->addWidget(_configurationView, 0, 1);

		_resultsView = new BenchmarkResultsView(benchmarkResultsModel);
		_ui->glMain->addWidget(_resultsView, 1, 1);

		UpdateUi();
	}

	void BenchmarksView::LoadBenchmarkTree()
	{
		for (auto& group : _benchmarksModel.GetBenchmarkGroups())
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

	BenchmarksView::~BenchmarksView()
	{
		delete _ui;
	}

	void BenchmarksView::on_twBenchmarks_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
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
		_benchmarksController.SetCurrentBenchmark(benchmarkModel);
		UpdateUi();
	}

	void BenchmarksView::UpdateUi()
	{
		if (_uiUpdating) return;
		_uiUpdating = true;
		_ui->pbRun->setEnabled(_benchmarksModel.GetSelectedBenchmark() != nullptr);
		_ui->chkUpload->setChecked(_benchmarksModel.IsUploadResults());
		_ui->chkOpenResult->setChecked(_benchmarksModel.IsOpenResult());
		_ui->chkOpenResult->setEnabled(_benchmarksModel.IsUploadResults());
		_ui->spnTimes->setValue(_benchmarksModel.GetIterationsToRun());
		_uiUpdating = false;
	}

	void BenchmarksView::on_chkUpload_stateChanged(int state)
	{
		_benchmarksController.SetUploadResults(state == Qt::Checked);
		UpdateUi();
	}

	void BenchmarksView::on_chkOpenResult_stateChanged(int state)
	{
		_benchmarksController.SetOpenResultAfterUpload(state == Qt::Checked);
		UpdateUi();
	}

	void BenchmarksView::on_spnTimes_valueChanged(int value)
	{
		_benchmarksController.SetIterationsToRun(value);
		UpdateUi();
	}

	void BenchmarksView::on_pbRun_clicked(bool checked)
	{
		Run();
	}

	void BenchmarksView::Run()
	{
		BenchmarkRunningDialog dialog(this);

		std::string exceptionMessage;
		std::atomic<bool> rejected = false;
		auto th = std::thread([this, &dialog, &exceptionMessage, &rejected]()
		{
		  try
		  {
			  _benchmarksController.Run();
		  }
		  catch (const std::exception& ex)
		  {
			  exceptionMessage = ex.what();
		  }

		  while (!rejected.load(std::memory_order::acquire) && !dialog.isVisible())
		  {
			  std::this_thread::sleep_for(MilliSeconds(10));
		  }
		  if (!rejected.load(std::memory_order::acquire))
		  {
			  dialog.accept();
		  }
		});
		if (dialog.exec() == QDialog::Rejected)
		{
			rejected.store(true, std::memory_order::release);
			_benchmarksController.StopRunning();
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