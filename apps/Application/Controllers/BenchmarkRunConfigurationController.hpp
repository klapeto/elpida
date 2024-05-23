//
// Created by klapeto on 20/10/2023.
//

#ifndef ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP
#define ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP

#include "Controller.hpp"

#include "Models/BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	class SettingsService;

	class BenchmarkRunConfigurationController final: public Controller<BenchmarkRunConfigurationModel>
	{
	public:
		void SetUploadResults(bool uploadResults);
		void SetOpenResultAfterUpload(bool openResult);
		void SetNumaAware(bool numaAware);
		void SetPinThreads(bool pinThreads);
		void SetIterationsToRun(std::size_t iterationsToRun);
		void SetConcurrencyMode(ConcurrencyMode concurrencyMode);

		explicit BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model, SettingsService& settingsService);
		~BenchmarkRunConfigurationController() override = default;
	private:
		SettingsService& _settingsService;
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP
