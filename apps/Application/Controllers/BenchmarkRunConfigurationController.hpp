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
		void SetGenerateHtmlReport(bool generateHtmlReport);
		void SetDelaySecondsBetweenRuns(std::size_t seconds);
		void SetMinimumMicroTaskDuration(Duration duration);

		explicit BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model, SettingsService& settingsService);
		~BenchmarkRunConfigurationController() override = default;
	private:
		SettingsService& _settingsService;
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP
