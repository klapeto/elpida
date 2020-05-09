#include "BenchmarkResultsView.hpp"
#include "ui_BenchmarkResultsView.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

#include <QTreeWidgetItem>

namespace Elpida
{

	BenchmarkResultsView::BenchmarkResultsView(const CollectionModel<BenchmarkResult>& model)
		:
		QWidget(), CollectionModelObserver<BenchmarkResult>(model), _ui(new Ui::BenchmarkResultsView)
	{
		_ui->setupUi(this);
	}

	BenchmarkResultsView::~BenchmarkResultsView()
	{
		delete _ui;
	}

	static std::string getResultString(const TaskResult& result)
	{
		auto& metrics = result.getMetrics();
		auto& resultSpec = result.getTaskSpecification().getResultSpecification();
		if (resultSpec.getType() == ResultSpecification::Throughput)
		{
			return Vu::Cs(Vu::getValueScaleStringSI(
				metrics.getResultValue() / metrics.getDurationSubdivision<Second>()),
				resultSpec.getUnit(),
				"/s");
		}
		else
		{
			return std::to_string(metrics.getResultValue()) + resultSpec.getUnit();
		}
	}

	void BenchmarkResultsView::onItemAdded(const BenchmarkResult& item)
	{

		auto parent = new QTreeWidgetItem();
		parent->setText(0, QString::fromStdString(item.getBenchmark().getName()));
		for (const auto& result: item.getTaskResults())
		{
			auto child = new QTreeWidgetItem();

			child->setText(0, QString::fromStdString(result.getTaskSpecification().getName()));
			child->setText(1,
				QString::fromStdString(getResultString(result)));
			parent->addChild(child);
		}
		_createdItems.emplace(item.getId(), parent);
		_ui->twResultList->addTopLevelItem(parent);
	}

	void BenchmarkResultsView::onItemRemoved(const BenchmarkResult& item)
	{
		auto itr = _createdItems.find(item.getId());
		if (itr != _createdItems.end())
		{
			_ui->twResultList->removeItemWidget(itr->second, 0);
			_createdItems.erase(itr);
		}
	}

	void BenchmarkResultsView::onCollectionCleared()
	{
		_ui->twResultList->clear();
		_createdItems.clear();
	}

} // namespace Elpida
