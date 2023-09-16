//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_
#define ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_

#include "Controller.hpp"
#include "Models/BenchmarkConfigurationModel.hpp"

namespace Elpida::Application
{

	class SettingsService;

	class BenchmarkConfigurationInstanceController : public Controller<BenchmarkConfigurationModel>
	{
	 public:
		void SetValue(std::string value);
		explicit BenchmarkConfigurationInstanceController(BenchmarkConfigurationModel& model, SettingsService& settingsService);
	private:
		SettingsService& _settingsService;
	};

} // Application

#endif //ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_
