//
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkConfigurationInstanceController.hpp"
#include "Core/SettingsService.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationInstanceController::BenchmarkConfigurationInstanceController(BenchmarkConfigurationModel& model, SettingsService& settingsService)
		: Controller(model), _settingsService(settingsService)
	{
	}

	void BenchmarkConfigurationInstanceController::SetValue(std::string value)
	{
		_settingsService.Set(_model.GetId(), value);
		_model.SetValue(std::move(value));
	}
} // Application