//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKSMODEL_HPP_
#define ELPIDA_BENCHMARKSMODEL_HPP_

#include "Models/Abstractions/Model.hpp"
#include "BenchmarkGroupModel.hpp"

namespace Elpida::Application
{
	class BenchmarksModel : public Model
	{
	 public:

		[[nodiscard]]
		const BenchmarkModel* GetSelectedBenchmark() const;

		void SetSelectedBenchmark(const BenchmarkModel* benchmark);

		void SetIterationsToRun(int iterations);
		void SetUploadResults(bool uploadResults);
		void SetOpenResult(bool openResult);

		[[nodiscard]]
		int GetIterationsToRun() const;

		[[nodiscard]]
		bool IsUploadResults() const;

		[[nodiscard]]
		bool IsOpenResult() const;

		[[nodiscard]]
		const std::vector<BenchmarkGroupModel>& GetBenchmarkGroups() const;

		explicit BenchmarksModel(std::vector<BenchmarkGroupModel>&& benchmarkGroups);
		~BenchmarksModel() override = default;
	 private:
		std::vector<BenchmarkGroupModel> _benchmarkGroups;
		const BenchmarkModel* _selectedBenchmark;
		int _iterationsToRun;
		bool _uploadResults;
		bool _openResult;
	};

} // Application

#endif //ELPIDA_BENCHMARKSMODEL_HPP_
