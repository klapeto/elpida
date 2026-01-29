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
// Created by klapeto on 19/10/2023.
//

#include "BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	BenchmarkRunConfigurationModel::BenchmarkRunConfigurationModel()
			:_iterationsToRun(1),
			 _delaySecondsBetweenRuns(0),
			 _minimumMicroTaskDuration(5.0),
			 _uploadResults(true),
			 _openResult(false), _numaAware(false), _pinThreads(false), _generateHtmlReport(false),
			 _concurrencyMode(ConcurrencyMode::None)
	{

	}

	std::size_t BenchmarkRunConfigurationModel::GetIterationsToRun() const
	{
		return _iterationsToRun;
	}

	bool BenchmarkRunConfigurationModel::IsUploadResults() const
	{
		return _uploadResults;
	}

	bool BenchmarkRunConfigurationModel::IsOpenResult() const
	{
		return _openResult;
	}

	void BenchmarkRunConfigurationModel::SetUploadResults(bool uploadResults)
	{
		_uploadResults = uploadResults;
		OnDataChanged();
	}

	void BenchmarkRunConfigurationModel::SetOpenResult(bool openResult)
	{
		_openResult = openResult;
		OnDataChanged();
	}

	bool BenchmarkRunConfigurationModel::IsNumaAware() const
	{
		return _numaAware;
	}

	void BenchmarkRunConfigurationModel::SetNumaAware(bool numaAware)
	{
		_numaAware = numaAware;
		OnDataChanged();
	}

	bool BenchmarkRunConfigurationModel::IsPinThreads() const
	{
		return _pinThreads;
	}

	void BenchmarkRunConfigurationModel::SetPinThreads(bool pinThreads)
	{
		_pinThreads = pinThreads;
		OnDataChanged();
	}

	ConcurrencyMode BenchmarkRunConfigurationModel::GetConcurrencyMode() const
	{
		return _concurrencyMode;
	}

	void BenchmarkRunConfigurationModel::SetConcurrencyMode(ConcurrencyMode concurrencyMode)
	{
		_concurrencyMode = concurrencyMode;
		OnDataChanged();
	}

	void BenchmarkRunConfigurationModel::SetIterationsToRun(std::size_t iterations)
	{
		_iterationsToRun = iterations;
		_iterationsChanged.Raise(_iterationsToRun);
		OnDataChanged();
	}

	void BenchmarkRunConfigurationModel::SetGenerateHtmlReport(bool generateHtmlReport)
	{
		_generateHtmlReport = generateHtmlReport;
		OnDataChanged();
	}

	bool BenchmarkRunConfigurationModel::IsGenerateHtmlReport() const
	{
		return _generateHtmlReport;
	}

	size_t BenchmarkRunConfigurationModel::GetDelaySecondsBetweenRuns() const
	{
		return _delaySecondsBetweenRuns;
	}

	void BenchmarkRunConfigurationModel::SetDelaySecondsBetweenRuns(size_t delaySecondsBetweenRuns)
	{
		_delaySecondsBetweenRuns = delaySecondsBetweenRuns;
		_delaySecondsBetweenRunsChanged.Raise(_delaySecondsBetweenRuns);
		OnDataChanged();
	}

	void BenchmarkRunConfigurationModel::SetMinimumMicroTaskDuration(Duration seconds)
	{
		_minimumMicroTaskDuration = seconds;
		OnDataChanged();
	}

	Duration BenchmarkRunConfigurationModel::GetMinimumMicroTaskDuration() const
	{
		return _minimumMicroTaskDuration;
	}
} // Elpida