//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 19/10/2023.
//

#ifndef ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP
#define ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/Duration.hpp"


namespace Elpida::Application
{
	enum class ConcurrencyMode
	{
		None = 0,
		CopyInput = 1,
		ShareInput = 1 << 1,
		ChunkInput = 1 << 2
	};

	class BenchmarkRunConfigurationModel final : public Model
	{
	public:
		void SetIterationsToRun(std::size_t iterations);
		void SetUploadResults(bool uploadResults);
		void SetOpenResult(bool openResult);
		void SetNumaAware(bool numaAware);
		void SetPinThreads(bool pinThreads);
		void SetGenerateHtmlReport(bool generateHtmlReport);
		void SetConcurrencyMode(ConcurrencyMode concurrencyMode);
		void SetDelaySecondsBetweenRuns(std::size_t delaySecondsBetweenRuns);
		void SetMinimumMicroTaskDuration(Duration seconds);

		[[nodiscard]]
		Duration GetMinimumMicroTaskDuration() const;

		[[nodiscard]]
		std::size_t GetDelaySecondsBetweenRuns() const;

		[[nodiscard]]
		bool IsPinThreads() const;

		[[nodiscard]]
		std::size_t GetIterationsToRun() const;

		[[nodiscard]]
		bool IsUploadResults() const;

		[[nodiscard]]
		bool IsOpenResult() const;

		[[nodiscard]]
		bool IsNumaAware() const;

		[[nodiscard]]
		bool IsGenerateHtmlReport() const;

		[[nodiscard]]
		ConcurrencyMode GetConcurrencyMode() const;

		BenchmarkRunConfigurationModel();

		~BenchmarkRunConfigurationModel() override = default;
	private:
		std::size_t _iterationsToRun;
		std::size_t _delaySecondsBetweenRuns;
		Duration _minimumMicroTaskDuration;
		bool _uploadResults;
		bool _openResult;
		bool _numaAware;
		bool _pinThreads;
		bool _generateHtmlReport;
		ConcurrencyMode _concurrencyMode;
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP
