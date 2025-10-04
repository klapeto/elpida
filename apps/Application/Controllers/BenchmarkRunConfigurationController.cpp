/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 20/10/2023.
//

#include "BenchmarkRunConfigurationController.hpp"
#include "Core/SettingsService.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include <string>

namespace Elpida::Application
{

	BenchmarkRunConfigurationController::BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model,
			SettingsService& settingsService)
			:Controller(model), _settingsService(settingsService)
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

		value = settingsService.Get("PinThreads");
		if (!value.empty())
		{
			_model.SetPinThreads(value == "1");
		}

		value = settingsService.Get("ConcurrencyMode");
		if (!value.empty())
		{
			_model.SetConcurrencyMode(static_cast<ConcurrencyMode>(std::stoul(value)));
		}
		else
		{
			_model.SetConcurrencyMode(ConcurrencyMode::None);
		}

		value = settingsService.Get("GenerateHtmlReport");
		if (!value.empty())
		{
			_model.SetGenerateHtmlReport(value == "1");
		}
		else
		{
			_model.SetGenerateHtmlReport(false);
		}

		value = settingsService.Get("DelaySecondsBetweenRuns");
		if (!value.empty())
		{
			_model.SetDelaySecondsBetweenRuns(static_cast<std::size_t>(std::stoul(value)));
		}

		value = settingsService.Get("MinimumMicroTaskDuration");
		if (!value.empty())
		{
			_model.SetMinimumMicroTaskDuration(Duration(std::stod(value)));
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

	void BenchmarkRunConfigurationController::SetConcurrencyMode(ConcurrencyMode concurrencyMode)
	{
		_model.SetConcurrencyMode(concurrencyMode);
		_settingsService.Set("ConcurrencyMode", std::to_string(static_cast<std::size_t>(concurrencyMode)));
	}

	void BenchmarkRunConfigurationController::SetGenerateHtmlReport(bool generateHtmlReport)
	{
		_model.SetGenerateHtmlReport(generateHtmlReport);
		_settingsService.Set("GenerateHtmlReport", generateHtmlReport ? "1" : "0");
	}

	void BenchmarkRunConfigurationController::SetDelaySecondsBetweenRuns(std::size_t seconds)
	{
		_model.SetDelaySecondsBetweenRuns(seconds);
		_settingsService.Set("DelaySecondsBetweenRuns", std::to_string(seconds));
	}

	void BenchmarkRunConfigurationController::SetMinimumMicroTaskDuration(Duration duration)
	{
		_model.SetMinimumMicroTaskDuration(duration);
		_settingsService.Set("MinimumMicroTaskDuration", std::to_string(duration.count()));
	}
} // Elpida
// Application