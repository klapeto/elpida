#include "CustomBenchmarkResultsView.hpp"
#include "ui_CustomBenchmarkResultsView.h"

#include "Elpida/Core/ResultType.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Models/Benchmark/TaskModel.hpp"
#include "Models/Custom/CustomBenchmarkModel.hpp"

#include <qtreewidget.h>
#include <string>

namespace Elpida::Application
{

	static std::string GetTaskValue(const TaskModel& task, const TaskResultModel& taskResult)
	{
		auto type = task.GetResultType();
		if (type == ResultType::Throughput)
		{

			return Elpida::ValueUtilities::GetValueScaleStringSI(
				(double)taskResult.GetInputSize() / taskResult.GetDuration().count()) + task.GetUnit() + +"/s";
		}
		else if (type == ResultType::Time)
		{
			return Elpida::ValueUtilities::GetValueScaleStringSI(taskResult.GetDuration().count()) + "s";
		}

		return Elpida::ValueUtilities::GetValueScaleStringSI((double)taskResult.GetInputSize()) + task.GetUnit();
	}

	CustomBenchmarkResultsView::CustomBenchmarkResultsView(const CustomBenchmarkModel& model, QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::CustomBenchmarkResultsView), _model(model)
	{
		_ui->setupUi(this);
		_resultAdded = model.ItemAdded().Subscribe([this](const CollectionItem<BenchmarkResultModel>& item)
		{
		  auto& result = item.GetValue();
		  auto& benchmark = result.GetBenchmark();
		  auto root = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
			  QStringList({ QString::fromStdString(benchmark.GetName()), QString::fromStdString(Elpida::ValueUtilities::GetValueScaleStringSI(result.GetResult()) + benchmark.GetResultUnit() +(benchmark.GetResultType() == ResultType::Throughput ? "/s" : "")) }));
		  _ui->twResults->addTopLevelItem(root);
		});

		_resultsCleared = model.Cleared().Subscribe([this]()
		{
			OnResultsCleared();
		});
	}

	CustomBenchmarkResultsView::~CustomBenchmarkResultsView()
	{
		delete _ui;
	}

	void CustomBenchmarkResultsView::OnResultsCleared()
	{
		_ui->twResults->clear();
	}

} // namespace Elpida
