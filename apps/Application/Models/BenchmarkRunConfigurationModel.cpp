//
// Created by klapeto on 19/10/2023.
//

#include "BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	BenchmarkRunConfigurationModel::BenchmarkRunConfigurationModel()
			: _iterationsToRun(1), _uploadResults(true), _openResult(false), _numaAware(false)
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

	void BenchmarkRunConfigurationModel::SetIterationsToRun(std::size_t iterations)
	{
		_iterationsToRun = iterations;
		OnDataChanged();
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
} // Elpida