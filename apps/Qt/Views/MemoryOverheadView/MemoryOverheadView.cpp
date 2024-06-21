#include "MemoryOverheadView.hpp"
#include "ui_MemoryOverheadView.h"

#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"
#include "Models/MemoryOverhead/MemoryOverheadCalculationModel.hpp"
#include "Controllers/MemoryOverheadCalculationController.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include <QTableWidgetItem>

namespace Elpida::Application
{
	MemoryOverheadView::MemoryOverheadView(
			const MemoryOverheadCalculationModel& model,
			MemoryOverheadCalculationController& controller,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController, QWidget* parent)
			:QWidget(parent), _ui(new Ui::MemoryOverheadView), _controller(controller), _model(model), _updatingUi(false), _currentBenchmarkIndex(0),_maxBenchmarkIndex(0)
	{
		_ui->setupUi(this);

		_runConfigurationView = new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel,
				benchmarkRunConfigurationController);

		_runConfigurationView->DisableIterations();
		_runConfigurationView->DisableUpload();
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
				//_cancel = false;
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

		_ui->spnInitialScale->setValue(_model.GetInitialScale());
		_ui->spnSubSamplesMultiplier->setValue(_model.GetSubSamplesMultiplier());
		_ui->spnScaleMultiplier->setValue(_model.GetScaleMultiplier());
		_ui->spnInitialSubSamples->setValue(_model.GetInitialSubSamples());
		_ui->spnIterations->setValue(_model.GetIterations());

		_updatingUi = false;
	}

	MemoryOverheadView::~MemoryOverheadView()
	{
		delete _ui;
	}

	void MemoryOverheadView::UpdateProgress()
	{
		_ui->lblProgress->setText(QString::fromStdString(
				std::to_string(_currentBenchmarkIndex) + "/" + std::to_string(_maxBenchmarkIndex)));
		_ui->pbProgress->setValue(_currentBenchmarkIndex);
	}

	void MemoryOverheadView::OnResultAdded(const MemoryOverheadResultModel& result)
	{
		auto workingSetSize = QString::fromStdString(ValueUtilities::ToIEC(result.GetWorkingSetSize(), 2) + "B");
		auto scale = QString::fromStdString(ValueUtilities::ToFixed(result.GetScale(), 6));
		auto subSamples = QString::fromStdString(std::to_string(result.GetSubSamples()));
		auto expectedScore = QString::fromStdString(ValueUtilities::ToFixed(result.GetExpectedScore(), 3));
		auto actualScore = QString::fromStdString(ValueUtilities::ToFixed(result.GetActualScore(), 3));
		auto overhead = QString::fromStdString((result.GetOverhead() >= 0.0 ? "+" : "") + ValueUtilities::ToFixed(result.GetOverhead(), 2) + "%");

		auto root = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr),QStringList({ workingSetSize, scale, subSamples, expectedScore, actualScore, overhead}));

		root->setForeground(5, QBrush(QColor(result.GetOverhead()  >= 0.0 ? "green" : "red")));
		_ui->twMemoryOverheadResults->addTopLevelItem(root);
	}

	void MemoryOverheadView::OnResultsCleared()
	{
		_ui->twMemoryOverheadResults->clear();
	}

	void MemoryOverheadView::on_bpStart_clicked(bool checked)
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

	void MemoryOverheadView::on_spnInitialScale_valueChanged(double value)
	{
		if (_updatingUi) return;
		_controller.SetInitialScale(value);
	}

	void MemoryOverheadView::on_spnInitialSubSamples_valueChanged(int value)
	{
		if (_updatingUi) return;
		_controller.SetInitialSubSamples(value);
	}

	void MemoryOverheadView::on_spnScaleMultiplier_valueChanged(double value)
	{
		if (_updatingUi) return;
		_controller.SetScaleMultiplier(value);
	}

	void MemoryOverheadView::on_spnSubSamplesMultiplier_valueChanged(double value)
	{
		if (_updatingUi) return;
		_controller.SetSubSamplesMultiplier(value);
	}

	void MemoryOverheadView::on_spnIterations_valueChanged(int value)
	{
		if (_updatingUi) return;
		_controller.SetIterations(value);
	}
}