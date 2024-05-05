#include "FullBenchmarkView.hpp"

#include "ui_FullBenchmarkView.h"
#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controllers/FullBenchmarkController.hpp"

namespace Elpida::Application
{
	using Vu = ValueUtilities;

	static void
	SetDelta(QLabel* label, FullBenchmarkResultModel::Score currentScore, FullBenchmarkResultModel::Score previousScore,
			QTreeWidgetItem* treeItem = nullptr)
	{
		if (previousScore == 0.0) return;
		auto delta = (currentScore / previousScore * 100.0) - 100.0;
		auto deltaStr = QString::fromStdString((delta >= 0.0 ? "+" : "") + Vu::ToFixed(delta, 2) + "%");
		label->setText(deltaStr);
		label->setStyleSheet(delta >= 0.0 ? "color: green" : "color: red");

		if (treeItem != nullptr)
		{
			treeItem->setText(4, deltaStr);
			treeItem->setForeground(4, QBrush(QColor(delta >= 0.0 ? "green" : "red")));
		}
	}

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

	FullBenchmarkView::FullBenchmarkView(
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			const FullBenchmarkModel& model,
			FullBenchmarkController& controller,
			QWidget* parent)
			:
			QWidget(parent),
			_ui(new Ui::FullBenchmarkView), _model(model), _controller(controller), _currentBenchmarkIndex(0),
			_maxBenchmarkIndex(0), _cancel(false)
	{
		_ui->setupUi(this);
		auto configurationView = new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel,
				benchmarkRunConfigurationController);
		configurationView->DisableAdvancedSettings();
		dynamic_cast<QVBoxLayout*>(_ui->gbExecution->layout())->insertWidget(2, configurationView);

		OnResultsCleared();

		_threadQueue = ThreadQueue::Current();
		_itemAdded = _model.ItemAdded().Subscribe([this](auto& item)
		{
			auto thread = _threadQueue.lock();
			if (!thread) return;
			thread->Enqueue([&]()
			{
				OnResultAdded(item.GetValue());
			});
		});
		_resultsCleared = _model.Cleared().Subscribe([this]()
		{
			OnResultsCleared();
		});

		_runningChanged = _model.RunningChanged().Subscribe([this](auto running)
		{
			auto thread = _threadQueue.lock();
			if (!thread) return;
			thread->Enqueue([this, running]()
			{
				if (running)
				{
					_currentBenchmarkIndex = 0;
					_maxBenchmarkIndex = _model.GetTotalBenchmarks();
					_ui->pbProgress->setRange(0, _maxBenchmarkIndex);
					_ui->bpStart->setText("Cancel");
					_ui->lblStatus->setText("Running...");
				}
				else
				{
					_currentBenchmarkIndex = 0;
					_maxBenchmarkIndex = 0;
					_ui->bpStart->setText("Start");
					_ui->lblStatus->setText("Ready");
				}
				UpdateProgress();
				_cancel = false;
				_ui->lblCurrentBenchmarkValue->setText("N/A");
			});
		});

		_currentBenchmarkChanged = _model.CurrentRunningBenchmarkChanged().Subscribe([this](auto& benchmark)
		{
			auto thread = _threadQueue.lock();
			if (!thread) return;
			thread->Enqueue([this, &benchmark]()
			{
				_ui->lblCurrentBenchmarkValue->setText(QString::fromStdString(benchmark));
				UpdateProgress();
				_currentBenchmarkIndex++;
			});
		});
	}

	FullBenchmarkView::~FullBenchmarkView()
	{
		delete _ui;
	}

	void FullBenchmarkView::UpdateProgress()
	{
		_ui->lblProgress->setText(QString::fromStdString(
				std::to_string(_currentBenchmarkIndex) + "/" + std::to_string(_maxBenchmarkIndex)));
		_ui->pbProgress->setValue(_currentBenchmarkIndex);
	}

	void FullBenchmarkView::on_bpStart_clicked(bool checked)
	{
		if (_model.IsRunning())
		{
			_controller.StopRunning();
		}
		else
		{
			_controller.RunAsync();
		}
	}

	void FullBenchmarkView::OnResultAdded(const FullBenchmarkResultModel& currentResult)
	{
		auto totalScore = QString::fromStdString(Vu::ToFixed(currentResult.GetTotalScore(), 2));
		auto singleScore = QString::fromStdString(Vu::ToFixed(currentResult.GetSingleCoreScore(), 2));
		auto multiScore = QString::fromStdString(Vu::ToFixed(currentResult.GetMultiCoreScore(), 2));
		auto memoryScore = QString::fromStdString(Vu::ToFixed(currentResult.GetMemoryScore(), 2));

		_ui->lblTotalScore->setText(totalScore);
		_ui->lblSingleCoreScoreValue->setText(singleScore);
		_ui->lblMultiCoreScoreValue->setText(multiScore);
		_ui->lblMemoryScoreValue->setText(memoryScore);

		auto item = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
				QStringList({ totalScore, singleScore, multiScore, memoryScore }));

		if (_previousScores.has_value())
		{
			auto& previousResult = *_previousScores;
			SetDelta(_ui->lblTotalScoreDelta, currentResult.GetTotalScore(), previousResult._totalScore, item);
			SetDelta(_ui->lblSingleScoreDelta, currentResult.GetSingleCoreScore(), previousResult._singleCoreScore);
			SetDelta(_ui->lblMultiCoreScoreDelta, currentResult.GetMultiCoreScore(),
					previousResult._multiCoreScore);
			SetDelta(_ui->lblMemoryScoreDelta, currentResult.GetMemoryScore(), previousResult._memoryScore);
		}

		_ui->twBenchmarkResults->addTopLevelItem(item);

		for (auto& benchmarkResult : currentResult.GetBenchmarkResults())
		{
			auto& benchmark = benchmarkResult.GetBenchmark();
			auto root = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
					QStringList({ QString::fromStdString(benchmark.GetName()),
								  QString::fromStdString(
										  Elpida::ValueUtilities::GetValueScaleStringSI(benchmarkResult.GetScore())
										  + benchmark.GetScoreUnit()) }));
			auto& taskResults = benchmarkResult.GetTaskResults();
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
			item->addChild(root);
		}

		_previousScores = {
				currentResult.GetTotalScore(),
				currentResult.GetSingleCoreScore(),
				currentResult.GetMultiCoreScore(),
				currentResult.GetMemoryScore(),
		};
	}

	void FullBenchmarkView::OnResultsCleared()
	{
		SetDelta(_ui->lblTotalScoreDelta, 1, 1);
		SetDelta(_ui->lblSingleScoreDelta, 1, 1);
		SetDelta(_ui->lblMultiCoreScoreDelta, 1, 1);
		SetDelta(_ui->lblMemoryScoreDelta, 1, 1);

		_ui->lblTotalScore->setText(_na);
		_ui->lblSingleCoreScoreValue->setText(_na);
		_ui->lblMultiCoreScoreValue->setText(_na);
		_ui->lblMemoryScoreValue->setText(_na);

		_ui->twBenchmarkResults->clear();
	}
}