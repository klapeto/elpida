//
// Created by klapeto on 19/10/2023.
//

#ifndef ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP
#define ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"


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
