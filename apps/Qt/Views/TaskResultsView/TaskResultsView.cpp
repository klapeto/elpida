#include "TaskResultsView.hpp"
#include "ui_TaskResultsView.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark.hpp>

#include <QTreeWidgetItem>

namespace Elpida
{

	TaskResultsView::TaskResultsView(const CollectionModel<BenchmarkResult>& model)
		:
		QWidget(), CollectionModelObserver<BenchmarkResult>(model),
		_ui(new Ui::TaskResultsView)
	{
		_ui->setupUi(this);
	}

	TaskResultsView::~TaskResultsView()
	{
		delete _ui;
	}

	void TaskResultsView::onItemAdded(const BenchmarkResult& item)
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

	void TaskResultsView::onItemRemoved(const BenchmarkResult& item)
	{
		auto itr = _createdItems.find(item.getId());
		if (itr != _createdItems.end())
		{
			_ui->twResultList->removeItemWidget(itr->second, 0);
			_createdItems.erase(itr);
		}
	}

	void TaskResultsView::onCollectionCleared()
	{
		_ui->twResultList->clear();
		_createdItems.clear();
	}

} // namespace Elpida
