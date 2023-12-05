//
// Created by klapeto on 19/10/2023.
//

#ifndef ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP
#define ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"


namespace Elpida::Application
{

	class BenchmarkRunConfigurationModel final : public Model
	{
	public:
		void SetIterationsToRun(std::size_t  iterations);

		void SetUploadResults(bool uploadResults);

		void SetOpenResult(bool openResult);

		void SetNumaAware(bool numaAware);

		[[nodiscard]]
		std::size_t GetIterationsToRun() const;

		[[nodiscard]]
		bool IsUploadResults() const;

		[[nodiscard]]
		bool IsOpenResult() const;

		[[nodiscard]]
		bool IsNumaAware() const;

		BenchmarkRunConfigurationModel();

		~BenchmarkRunConfigurationModel() override = default;
	private:
		std::size_t _iterationsToRun;
		bool _uploadResults;
		bool _openResult;
		bool _numaAware;
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKRUNCONFIGURATIONMODEL_HPP
