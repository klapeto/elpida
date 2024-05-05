//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_CUSTOMBENCHMARKSMODEL_HPP_
#define ELPIDA_CUSTOMBENCHMARKSMODEL_HPP_

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Abstractions/Model.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{
	class CustomBenchmarkModel : public ListModel<BenchmarkResultModel>
	{
	 public:

		[[nodiscard]]
		const BenchmarkModel* GetSelectedBenchmark() const;

		void SetSelectedBenchmark(const BenchmarkModel* benchmark);

		[[nodiscard]]
		const std::vector<BenchmarkGroupModel>& GetBenchmarkGroups() const;

		explicit CustomBenchmarkModel(const std::vector<BenchmarkGroupModel>& benchmarkGroups);
		~CustomBenchmarkModel() override = default;
	 private:
		std::vector<BenchmarkGroupModel> _benchmarkGroups;
		const BenchmarkModel* _selectedBenchmark;
	};

} // Application

#endif //ELPIDA_CUSTOMBENCHMARKSMODEL_HPP_
