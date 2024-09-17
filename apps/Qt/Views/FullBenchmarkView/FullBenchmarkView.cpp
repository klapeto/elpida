#include "FullBenchmarkView.hpp"

#include "ui_FullBenchmarkView.h"
#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controllers/FullBenchmarkController.hpp"

namespace Elpida::Application
{
	using Vu = ValueUtilities;

	QString GetDeltaString(Score delta)
	{
		return QString::fromStdString((delta >= 0.0 ? "+" : "") + Vu::ToFixed(delta, 2) + "%");
	}

	Score GetDelta(Score previousScore, Score currentScore)
	{
		return (currentScore / previousScore * 100.0) - 100.0;
	}

	static void
	SetTreeItemDelta(Score currentScore, Score previousScore,
			QTreeWidgetItem* treeItem)
	{
		if (previousScore == 0.0) return;
		auto delta = GetDelta(previousScore, currentScore);
		auto deltaStr = GetDeltaString(delta);

		treeItem->setText(3, deltaStr);
		treeItem->setForeground(3, QBrush(QColor(delta >= 0.0 ? "green" : "red")));
	}

	static void
	SetDelta(QLabel* label, Score currentScore, Score previousScore,
			QTreeWidgetItem* treeItem = nullptr)
	{
		if (previousScore == 0.0) return;
		auto delta = GetDelta(previousScore, currentScore);

		auto deltaStr = GetDeltaString(delta);
		label->setText(deltaStr);
		label->setStyleSheet(delta >= 0.0 ? "color: green" : "color: red");

		if (treeItem != nullptr)
		{
			treeItem->setText(3, deltaStr);
			treeItem->setForeground(3, QBrush(QColor(delta >= 0.0 ? "green" : "red")));
		}
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
				if (benchmark != "Waiting..." && benchmark != "Uploading...")    //bad but will work for now
				{
					UpdateProgress();
					_currentBenchmarkIndex++;
				}
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
		auto singleScore = QString::fromStdString(Vu::ToFixed(currentResult.GetSingleThreadScore(), 2));
		auto multiScore = QString::fromStdString(Vu::ToFixed(currentResult.GetMultiThreadScore(), 2));

		_ui->lblTotalScore->setText(totalScore);
		_ui->lblSingleThreadScoreValue->setText(singleScore);
		_ui->lblMultiThreadScoreValue->setText(multiScore);

		auto item = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
				QStringList({ totalScore, singleScore, multiScore }));

		if (_previousScore.has_value())
		{
			auto& previousResult = *_previousScore;
			SetDelta(_ui->lblTotalScoreDelta, currentResult.GetTotalScore(), previousResult.GetTotalScore(), item);
			SetDelta(_ui->lblSingleThreadScoreDelta, currentResult.GetSingleThreadScore(),
					previousResult.GetSingleThreadScore());
			SetDelta(_ui->lblMultiThreadScoreDelta, currentResult.GetMultiThreadScore(),
					previousResult.GetMultiThreadScore());
		}

		_ui->twBenchmarkResults->addTopLevelItem(item);

		auto& currentBenchmarkResults = currentResult.GetBenchmarkResults();
		for (std::size_t i = 0; i < currentBenchmarkResults.size(); ++i)
		{
			auto& benchmarkResult = currentBenchmarkResults[i];
			auto& benchmark = benchmarkResult.GetBenchmark();
			auto root = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
					QStringList({
							QString::fromStdString(benchmark.GetName()),
							QString::fromStdString(
									Elpida::ValueUtilities::GetValueScaleStringSI(benchmarkResult.GetResult())
									+ benchmark.GetResultUnit() + (benchmark.GetResultType() == ResultType::Throughput ? "/s" : ""))
					}));
			if (_previousScore.has_value())
			{
				auto& previousScore = _previousScore.value().GetBenchmarkResults()[i];
				SetTreeItemDelta(benchmarkResult.GetResult(), previousScore.GetResult(), root);
			}
			item->addChild(root);
		}

		_previousScore = currentResult;
	}

	void FullBenchmarkView::OnResultsCleared()
	{
		SetDelta(_ui->lblTotalScoreDelta, 1, 1);
		SetDelta(_ui->lblSingleThreadScoreDelta, 1, 1);
		SetDelta(_ui->lblMultiThreadScoreDelta, 1, 1);

		_ui->lblTotalScore->setText(_na);
		_ui->lblSingleThreadScoreValue->setText(_na);
		_ui->lblMultiThreadScoreValue->setText(_na);

		_ui->twBenchmarkResults->clear();

		_previousScore.reset();
	}
}