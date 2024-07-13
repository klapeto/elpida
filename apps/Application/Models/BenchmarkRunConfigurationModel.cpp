//
// Created by klapeto on 19/10/2023.
//

#include "BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	BenchmarkRunConfigurationModel::BenchmarkRunConfigurationModel()
			: _iterationsToRun(1), _uploadResults(true), _openResult(false), _numaAware(false), _pinThreads(false), _generateHtmlReport(false), _concurrencyMode(ConcurrencyMode::None)
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
	}

	void BenchmarkRunConfigurationModel::SetIterationsToRun(std::size_t iterations)
	{
		_iterationsToRun = iterations;
		OnDataChanged();
	}

	void BenchmarkRunConfigurationModel::SetGenerateHtmlReport(bool generateHtmlReport)
	{
		_generateHtmlReport = generateHtmlReport;
	}

	bool BenchmarkRunConfigurationModel::IsGenerateHtmlReport() const
	{
		return _generateHtmlReport;
	}
} // Elpida