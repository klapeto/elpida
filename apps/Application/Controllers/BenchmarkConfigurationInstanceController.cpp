//
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationInstanceController::BenchmarkConfigurationInstanceController(BenchmarkConfigurationModel& model)
		: Controller(model)
	{
	}

	void BenchmarkConfigurationInstanceController::SetValue(std::string value)
	{
		_model.SetValue(std::move(value));
	}
} // Application