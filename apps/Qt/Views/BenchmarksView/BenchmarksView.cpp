#include "BenchmarksView.hpp"
#include "ui_BenchmarksView.h"

#include "Models/BenchmarksModel.hpp"
#include "Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp"

namespace Elpida::Application
{
	BenchmarksView::BenchmarksView(BenchmarksModel& benchmarksModel)
		: QWidget(), _ui(new Ui::BenchmarksView), _benchmarksModel(benchmarksModel)
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

		_ui->verticalLayout->addWidget(_configurationView);
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
	}
}