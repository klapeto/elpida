#include "BenchmarksView.hpp"
#include "ui_BenchmarksView.h"

#include "Models/BenchmarksModel.hpp"
#include "Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp"
#include "Views/BenchmarkResultsView/BenchmarkResultsView.hpp"

namespace Elpida::Application
{
	BenchmarksView::BenchmarksView(BenchmarksModel& benchmarksModel)
		: QWidget(), _ui(new Ui::BenchmarksView), _benchmarksModel(benchmarksModel), _uiUpdating(false)
	{
		_ui->setupUi(this);

		for (auto& group: benchmarksModel.GetBenchmarkGroups())
		{
			auto groupItem = new QTreeWidgetItem(QStringList(QString::fromStdString(group.GetName())));

			for (auto& benchmark: group.GetBenchmarks())
			{
				auto key = QString::fromStdString(group.GetName() + benchmark.GetName());
				_benchmarkMap.insert({ key, const_cast<BenchmarkModel*>(&benchmark) });
				auto item = new QTreeWidgetItem(QStringList(QString::fromStdString(benchmark.GetName())));
				item->setData(1, 0, QVariant(key));
				groupItem->addChild(item);
			}

			_ui->twBenchmarks->addTopLevelItem(groupItem);
		}

		_configurationView = new BenchmarkConfigurationView();

		_ui->glMain->addWidget(_configurationView, 0, 1);

		_resultsView = new BenchmarkResultsView();
		_ui->glMain->addWidget(_resultsView, 1, 1);

		UpdateUi();
	}

	BenchmarksView::~BenchmarksView()
	{
		delete _ui;
	}

	void BenchmarksView::on_twBenchmarks_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
	{
		if (current != nullptr)
		{
			auto key = current->data(1, 0).value<QString>();
			auto itr = _benchmarkMap.find(key);

			if (itr != _benchmarkMap.end())
			{
				_benchmarksModel.SetSelectedBenchmark(itr->second);
				_configurationView->SetModel(itr->second);
			}
			else
			{
				_benchmarksModel.SetSelectedBenchmark(nullptr);
				_configurationView->SetModel(nullptr);
			}
		}
		else
		{
			_benchmarksModel.SetSelectedBenchmark(nullptr);
			_configurationView->SetModel(nullptr);
		}
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
		_benchmarksModel.SetUploadResults(state == Qt::Checked);
		UpdateUi();
	}

	void BenchmarksView::on_chkOpenResult_stateChanged(int state)
	{
		_benchmarksModel.SetOpenResult(state == Qt::Checked);
		UpdateUi();
	}

	void BenchmarksView::on_spnTimes_valueChanged(int value)
	{
		_benchmarksModel.SetIterationsToRun(value);
		UpdateUi();
	}
}