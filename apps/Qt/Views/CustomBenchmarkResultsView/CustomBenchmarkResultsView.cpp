#include "CustomBenchmarkResultsView.hpp"
#include "ui_CustomBenchmarkResultsView.h"

#include "Elpida/Core/ScoreType.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Models/Benchmark/TaskModel.hpp"
#include "Models/Custom/CustomBenchmarkResultsModel.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"

#include <qtreewidget.h>
#include <string>

namespace Elpida::Application
{

	static std::string GetTaskValue(const TaskModel& task, const TaskResultModel& taskResult)
	{
		auto type = task.GetScoreType();
		if (type == ScoreType::Throughput)
		{

			return Elpida::ValueUtilities::GetValueScaleStringSI(
				(double)taskResult.GetInputSize() / taskResult.GetDuration().count()) + task.GetUnit() + +"/s";
		}
		else if (type == ScoreType::Time)
		{
			return Elpida::ValueUtilities::GetValueScaleStringSI(taskResult.GetDuration().count()) + "s";
		}

		return Elpida::ValueUtilities::GetValueScaleStringSI((double)taskResult.GetInputSize()) + task.GetUnit();
	}

	CustomBenchmarkResultsView::CustomBenchmarkResultsView(const CustomBenchmarkResultsModel& model, QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::CustomBenchmarkResultsView), _model(model)
	{
		_ui->setupUi(this);
		_resultAdded = model.ItemAdded().Subscribe([this](const CollectionItem<BenchmarkResultModel>& item)
		{
		  auto& result = item.GetValue();
		  auto& benchmark = result.GetBenchmark();
		  auto root = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
			  QStringList({ QString::fromStdString(benchmark.GetName()),
							QString::fromStdString(Elpida::ValueUtilities::GetValueScaleStringSI(result.GetScore())
								+ benchmark.GetScoreUnit()) }));
		  auto& taskResults = result.GetTaskResults();
		  auto& tasks = benchmark.GetTasks();
		  for (std::size_t i = 0, j = 0; i < tasks.size(); ++i)
		  {
			  auto& task = tasks[i];
			  if (!task.IsMeasured()) continue;
			  auto& taskResult = taskResults[j++];

			  auto taskItem = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
				  QStringList({ QString::fromStdString(task.GetName()),
								QString::fromStdString(GetTaskValue(task, taskResult)) }));
			  root->addChild(taskItem);
		  }
		  _ui->twResults->addTopLevelItem(root);
		});

		_resultsCleared = model.Cleared().Subscribe([this]()
		{
		  _ui->twResults->clear();
		});
	}

	CustomBenchmarkResultsView::~CustomBenchmarkResultsView()
	{
		delete _ui;
	}

} // namespace Elpida
