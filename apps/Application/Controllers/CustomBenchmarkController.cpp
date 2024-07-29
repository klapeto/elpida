//
// Created by klapeto on 16/4/2023.
//


#include "CustomBenchmarkController.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include "Models/Custom/CustomBenchmarkModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/MessageService.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Core/PathsService.hpp"
#include "Core/DesktopService.hpp"

#include "ResultSerializer.hpp"

#include <fstream>
#include <string>
#include <thread>

namespace Elpida::Application
{
	CustomBenchmarkController::CustomBenchmarkController(CustomBenchmarkModel& model,
			TopologyModel& topologyModel, TimingModel& overheadsModel,
			BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService, const ResultSerializer& resultSerializer,
			const ResultsHTMLReporter& resultsHtmlReporter, const PathsService& pathsService,
			const DesktopService& desktopService,
			MessageService& messageService)
		:Controller<CustomBenchmarkModel>(model),
		 _topologyModel(topologyModel),
		 _overheadsModel(overheadsModel),
		 _benchmarkRunConfigurationModel(benchmarkRunConfigurationModel),
		 _benchmarkExecutionService(benchmarkExecutionService),
		 _resultSerializer(resultSerializer),
		 _resultsHTMLReporter(resultsHtmlReporter),
		 _pathsService(pathsService),
		 _desktopService(desktopService),
		 _messageService(messageService),
		 _cancelling(false)
	{

	}

	void CustomBenchmarkController::SetCurrentBenchmark(const BenchmarkModel* currentBenchmark)
	{
		_model.SetSelectedBenchmark(currentBenchmark);
	}

	void CustomBenchmarkController::Run()
	{
		_cancelling = false;
		auto selectedBenchmark = _model.GetSelectedBenchmark();
		if (selectedBenchmark == nullptr)
		{
			throw ElpidaException("You have to select a benchmark first.");
		}


		std::vector<std::size_t> affinity;
		std::vector<BenchmarkResultModel> thisRunResults;

		try
		{
			affinity.reserve(_topologyModel.GetSelectedLeafNodes().size());
			for (auto& node : _topologyModel.GetSelectedLeafNodes())
			{
				affinity.push_back(node.get().GetOsIndex().value());
			}

			for (std::size_t i = 0; i < _benchmarkRunConfigurationModel.GetIterationsToRun(); ++i)
			{
				BenchmarkResultModel result = _benchmarkExecutionService.Execute(
						*selectedBenchmark,
						affinity,
						_overheadsModel.GetNowOverhead().count(),
						_overheadsModel.GetLoopOverhead().count(),
						_benchmarkRunConfigurationModel.IsNumaAware(),
						_benchmarkRunConfigurationModel.IsPinThreads(),
						_benchmarkRunConfigurationModel.GetConcurrencyMode());

				if (_benchmarkRunConfigurationModel.IsGenerateHtmlReport())
				{
					thisRunResults.push_back(result);
				}
				_model.Add(std::move(result));

				if (_cancelling)
				{
					break;
				}

				if (_benchmarkRunConfigurationModel.GetDelaySecondsBetweenRuns() > 0
					&& _benchmarkRunConfigurationModel.GetIterationsToRun() > i + 1)
				{
					std::this_thread::sleep_for(Seconds(_benchmarkRunConfigurationModel.GetDelaySecondsBetweenRuns()));
				}

				if (_cancelling)
				{
					break;
				}
			}
		}
		catch (const ElpidaException& ex)
		{
			if (!_cancelling)
			{
				throw;
			}
		}

		if (_benchmarkRunConfigurationModel.IsGenerateHtmlReport() && !thisRunResults.empty())
		{
			GenerateHtmlReport(selectedBenchmark, affinity, thisRunResults);
		}
	}

	void CustomBenchmarkController::GenerateHtmlReport(const BenchmarkModel* selectedBenchmark,
			const std::vector<std::size_t>& affinity, const std::vector<BenchmarkResultModel>& thisRunResults) const
	{
		try
		{
			std::string fileName = "Custom ";
			fileName
					.append(selectedBenchmark->GetName())
					.append(" ")
					.append(std::to_string(time(nullptr)))
					.append(".html");

			auto path = _pathsService.GetDownloadStoragePath() / "Elpida Exported Reports" / fileName;

			_resultsHTMLReporter.WriteCustomBenchmarkReport(thisRunResults, affinity, path);

			_desktopService.OpenFile(path);
		}
		catch (const std::exception& ex)
		{
			_messageService.ShowError("Failed to create report: " + std::string(ex.what()));
		}
	}

	void CustomBenchmarkController::StopRunning()
	{
		_cancelling = true;
		_benchmarkExecutionService.StopCurrentExecution();
	}

	void CustomBenchmarkController::ClearResults()
	{
		_model.Clear();
	}

	void CustomBenchmarkController::SaveResults(const std::filesystem::path& filePath)
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);
		file << _resultSerializer.Serialize(_model);
	}

	void CustomBenchmarkController::GenerateReport(const std::filesystem::path& filePath)
	{
		//_ResultsHTMLReporter.WriteCustomBenchmarkReport()
	}
} // Application