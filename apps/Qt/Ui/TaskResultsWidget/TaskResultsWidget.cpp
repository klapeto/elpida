#include "TaskResultsWidget.hpp"
#include "ui_TaskResultsWidget.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark.hpp>

#include <QTreeWidgetItem>

namespace Elpida
{

	TaskResultsWidget::TaskResultsWidget(const CollectionModel<BenchmarkResult>& model)
		:
		QWidget(), CollectionModelObserver<BenchmarkResult>(model),
		_ui(new Ui::TaskResultsWidget)
	{
		_ui->setupUi(this);
	}

	TaskResultsWidget::~TaskResultsWidget()
	{
		delete _ui;
	}

	void TaskResultsWidget::onItemAdded(const BenchmarkResult& item)
	{
		auto parent = new QTreeWidgetItem();
		parent->setText(0, QString::fromStdString(item.getBenchmark().getName()));
		for (const auto& result: item.getTaskResults())
		{
			auto metrics = result.getMetrics();
			auto child = new QTreeWidgetItem();
			child->setText(0, QString::fromStdString(result.getSpecification().getName()));
			child->setText(1,
				QString::fromStdString(std::to_string(
					metrics.getActualProcessDataSize() / metrics.getDurationSubdivision<TaskMetrics::Second>())
					+ result.getSpecification().getOutputValueUnit() + "/s"));
			parent->addChild(child);
		}
		_createdItems.emplace(item.getId(), parent);
		_ui->twResultList->addTopLevelItem(parent);
	}

	void TaskResultsWidget::onItemRemoved(const BenchmarkResult& item)
	{
		auto itr = _createdItems.find(item.getId());
		if (itr != _createdItems.end())
		{
			_ui->twResultList->removeItemWidget(itr->second, 0);
			_createdItems.erase(itr);
		}
	}

	void TaskResultsWidget::onCollectionCleared()
	{
		_ui->twResultList->clear();
		_createdItems.clear();
	}

} // namespace Elpida
