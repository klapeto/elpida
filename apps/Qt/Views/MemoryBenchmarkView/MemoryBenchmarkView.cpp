#include "MemoryBenchmarkView.hpp"
#include "ui_MemoryBenchmarkView.h"

#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"
#include "Models/MemoryBenchmark/MemoryBenchmarkModel.hpp"
#include "Controllers/MemoryBenchmarkController.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

namespace Elpida::Application
{
	MemoryBenchmarkView::MemoryBenchmarkView(
			const MemoryBenchmarkModel& model,
			MemoryBenchmarkController& controller,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController, QWidget* parent)
			:QWidget(parent), _ui(new Ui::MemoryBenchmarkView),
			 _controller(controller),
			 _model(model),
			 _updatingUi(false),
			 _currentBenchmarkIndex(0),
			 _maxBenchmarkIndex(0),
			 _maxScore(std::numeric_limits<Score>::lowest()),
			 _minScore(std::numeric_limits<Score>::max()),
			 _minWorkingSet(std::numeric_limits<double>::max()),
			 _maxWorkingSet(std::numeric_limits<double>::lowest())
	{
		_ui->setupUi(this);

		_runConfigurationView = new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel,
				benchmarkRunConfigurationController);

		_runConfigurationView->DisableIterations();
		_runConfigurationView->DisableDelay();
		_runConfigurationView->DisableUpload();
		_runConfigurationView->DisableHtmlReport();
		static_cast<QVBoxLayout*>(_ui->gbExecution->layout())->insertWidget(2, _runConfigurationView);

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
					OnBenchmarkStarted();
				}
				else
				{
					OnBenchmarkStopped();
				}

				UpdateProgress();
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

		_updatingUi = true;

		_ui->spnStartSize->setValue(_model.GetInitialBytes());
		_ui->spnMultiplier->setValue(_model.GetMultiplier());
		_ui->spnIterations->setValue(_model.GetIterations());

		_series = new QLineSeries();

		_chart = new QChart();
		_chart->addSeries(_series);

		auto* axisX = new QLogValueAxis();
		axisX->setTitleText("Working set size (MiB)");
		axisX->setBase(8.0);
		axisX->setLabelsVisible(true);
		axisX->setLabelFormat("%.3f MiB");
		axisX->setMinorTickCount(-1);
		_chart->addAxis(axisX, Qt::AlignBottom);
		_series->attachAxis(axisX);

		auto* axisY = new QLogValueAxis();
		axisY->setMinorTickCount(-1);
		axisY->setLabelFormat("%.2f ns");
		_chart->addAxis(axisY, Qt::AlignLeft);
		_series->attachAxis(axisY);

		auto chartView = new QChartView();
		chartView->setChart(_chart);
		chartView->setRenderHint(QPainter::Antialiasing);
		static_cast<QGridLayout*>(this->layout())->addWidget(chartView, 0, 2, 3,1);

		static_cast<QGridLayout*>(this->layout())->setColumnStretch(2,1);

		switch (_model.GetBenchmarkType())
		{
		case MemoryBenchmarkType::MemoryLatency:
			_ui->rbMemoryLatency->setChecked(true);
			SetMemoryLatencyUi();
			break;
		default:
			_ui->rbMemoryReadBandwidth->setChecked(true);
			SetMemoryBandwidthUi();
			break;
		}
		_updatingUi = false;
	}

	void MemoryBenchmarkView::OnBenchmarkStarted()
	{
		_currentBenchmarkIndex = 0;
		_maxBenchmarkIndex = _model.GetTotalBenchmarks();
		_ui->pbProgress->setRange(0, _maxBenchmarkIndex);
		_ui->bpStart->setText("Cancel");
		_ui->lblStatus->setText("Running...");
	}

	void MemoryBenchmarkView::OnBenchmarkStopped()
	{
		_currentBenchmarkIndex = 0;
		_maxBenchmarkIndex = 0;
		_ui->bpStart->setText("Start");
		_ui->lblStatus->setText("Ready");
		ResetScores();
	}

	void MemoryBenchmarkView::SetMemoryBandwidthUi()
	{
		_chart->setTitle("Memory read bandwidth");
		_chart->axes(Qt::Vertical)[0]->setTitleText("Bandwidth (b/s)");
		static_cast<QLogValueAxis*>(_chart->axes(Qt::Vertical)[0])->setLabelFormat("%.2f Gib/s");
		_series->clear();
	}

	void MemoryBenchmarkView::SetMemoryLatencyUi()
	{
		_chart->setTitle("Memory latency");
		_chart->axes(Qt::Vertical)[0]->setTitleText("Latency (ns)");
		static_cast<QLogValueAxis*>(_chart->axes(Qt::Vertical)[0])->setLabelFormat("%.2f ns");
		_series->clear();
	}

	MemoryBenchmarkView::~MemoryBenchmarkView()
	{
		delete _ui;
	}

	void MemoryBenchmarkView::UpdateProgress()
	{
		_ui->lblProgress->setText(QString::fromStdString(
				std::to_string(_currentBenchmarkIndex) + "/" + std::to_string(_maxBenchmarkIndex)));
		_ui->pbProgress->setValue(_currentBenchmarkIndex);
	}

	static double GetMinimumValueForChart(double value)
	{
		auto exp = std::floor(std::log10(value));
		return std::pow(10, exp);
	}

	static double GetMaximumValueForChart(double value)
	{
		auto exp = std::ceil(std::log10(value));
		return std::pow(10, exp);
	}

	void MemoryBenchmarkView::OnResultAdded(const MemoryBenchmarkResultModel& result)
	{
		QString stringScore;
		Score scoreToShow = 0.0;
		if (_model.GetBenchmarkType() == MemoryBenchmarkType::MemoryLatency)
		{
			OnMemoryLatencyResultAdded(result, stringScore, scoreToShow);
		}
		else
		{
			OnMemoryBandwidthResultAdded(result, stringScore, scoreToShow);
		}

		auto workingSetSize = QString::fromStdString(ValueUtilities::ToIEC(result.GetWorkingSetSize(), 2) + "B");

		_ui->twMemoryBenchmarkResults->addTopLevelItem(new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),
				QStringList({ workingSetSize, stringScore })));

		auto horizontalAxe = _chart->axes(Qt::Horizontal)[0];
		auto verticalAxe = _chart->axes(Qt::Vertical)[0];

		auto calculatedWorkingSetSize = result.GetWorkingSetSize() / (1024.0 * 1024.0);
		_minWorkingSet = std::min(_minWorkingSet, calculatedWorkingSetSize);
		_maxWorkingSet = std::max(_maxWorkingSet, calculatedWorkingSetSize);

		horizontalAxe->setRange(_minWorkingSet - std::abs(_minWorkingSet * 0.1),_maxWorkingSet + std::abs(_maxWorkingSet * 0.1));

		_maxScore = std::max(_maxScore, scoreToShow);
		_minScore = std::min(_minScore, scoreToShow);

		verticalAxe->setRange(GetMinimumValueForChart(_minScore) * 0.9, GetMaximumValueForChart(_maxScore) * 1.1);

		*_series << QPointF(calculatedWorkingSetSize, scoreToShow);
	}

	void MemoryBenchmarkView::OnResultsCleared()
	{
		_ui->twMemoryBenchmarkResults->clear();
		_series->clear();
	}

	void MemoryBenchmarkView::on_bpStart_clicked(bool checked)
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

	void MemoryBenchmarkView::on_spnMultiplier_valueChanged(double value)
	{
		if (_updatingUi) return;
		_controller.SetMultiplier(value);
	}

	void MemoryBenchmarkView::on_spnStartSize_valueChanged(int value)
	{
		if (_updatingUi) return;
		_controller.SetInitialBytes(value);
	}

	void MemoryBenchmarkView::on_spnIterations_valueChanged(int value)
	{
		if (_updatingUi) return;
		_controller.SetIterations(value);
	}

	void MemoryBenchmarkView::on_rbMemoryLatency_toggled(bool value)
	{
		if (_updatingUi) return;
		if (value)
		{
			SetMemoryLatencyUi();
			_controller.SetBenchmarkType(MemoryBenchmarkType::MemoryLatency);
		}
	}

	void MemoryBenchmarkView::on_rbMemoryReadBandwidth_toggled(bool value)
	{
		if (_updatingUi) return;
		if (value)
		{
			SetMemoryBandwidthUi();
			_controller.SetBenchmarkType(MemoryBenchmarkType::MemoryReadBandwidth);
		}
	}

	void MemoryBenchmarkView::OnMemoryBandwidthResultAdded(const MemoryBenchmarkResultModel& result,
			QString& stringScore, Score& scoreToShow)
	{
		stringScore = QString::fromStdString(ValueUtilities::ToIEC(result.GetScore(), 2) + "b/s");

		scoreToShow = result.GetScore() / std::giga::num;

		_maxScore = std::max(_maxScore, scoreToShow);
		_minScore = std::min(_minScore, scoreToShow);
	}

	void MemoryBenchmarkView::OnMemoryLatencyResultAdded(const MemoryBenchmarkResultModel& result,
			QString& stringScore,
			Score& scoreToShow)
	{
		stringScore = QString::fromStdString(ValueUtilities::ToSI(result.GetScore(), 2) + "s");

		scoreToShow = std::chrono::duration_cast<NanoSeconds>(Seconds(result.GetScore())).count();

		_maxScore = std::max(_maxScore, scoreToShow);
		_minScore = std::min(_minScore, scoreToShow);
	}

	void MemoryBenchmarkView::ResetScores()
	{
		_maxScore = std::numeric_limits<Score>::lowest();
		_minScore = std::numeric_limits<Score>::max();
		_minWorkingSet = std::numeric_limits<double>::max();
		_maxWorkingSet = std::numeric_limits<double>::lowest();
	}
}