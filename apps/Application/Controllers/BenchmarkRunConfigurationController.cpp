//
// Created by klapeto on 20/10/2023.
//

#include "BenchmarkRunConfigurationController.hpp"
#include "Core/SettingsService.hpp"
#include <string>

namespace Elpida::Application
{
	BenchmarkRunConfigurationController::BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model,
		SettingsService& settingsService)
		: Controller(model), _settingsService(settingsService)
	{
		std::string value = settingsService.Get("UploadResults");
		if (!value.empty())
		{
			_model.SetUploadResults(value == "1");
		}

		value = settingsService.Get("OpenResults");
		if (!value.empty())
		{
			_model.SetOpenResult(value == "1");
		}

		value = settingsService.Get("IterationsToRun");
		if (!value.empty())
		{
			auto number = std::strtol(value.c_str(), nullptr, 10);
			if (number > 0)
			{
				_model.SetIterationsToRun(number);
			}
		}

		value = settingsService.Get("NumaAware");
		if (!value.empty())
		{
			_model.SetNumaAware(value == "1");
		}
	}

	void BenchmarkRunConfigurationController::SetUploadResults(bool uploadResults)
	{
		_model.SetUploadResults(uploadResults);
		_settingsService.Set("UploadResults", uploadResults ? "1" : "0");
	}

	void BenchmarkRunConfigurationController::SetOpenResultAfterUpload(bool openResult)
	{
		_model.SetOpenResult(openResult);
		_settingsService.Set("OpenResults", openResult ? "1" : "0");
	}

	void BenchmarkRunConfigurationController::SetIterationsToRun(std::size_t iterationsToRun)
	{
		_model.SetIterationsToRun(iterationsToRun);
		_settingsService.Set("IterationsToRun", std::to_string(iterationsToRun));
	}

	void BenchmarkRunConfigurationController::SetNumaAware(bool numaAware)
	{
		_model.SetNumaAware(numaAware);
		_settingsService.Set("NumaAware", numaAware ? "1" : "0");
	}

	void BenchmarkRunConfigurationController::SetPinThreads(bool pinThreads)
	{
		_model.SetPinThreads(pinThreads);
		_settingsService.Set("PinThreads", pinThreads ? "1" : "0");
	}
} // Elpida
// Application