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

		[[nodiscard]]
		const std::vector<BenchmarkGroupModel>& GetBenchmarkGroups() const;

		explicit BenchmarksModel(std::vector<BenchmarkGroupModel>&& benchmarkGroups);
		~BenchmarksModel() override = default;
	 private:
		std::vector<BenchmarkGroupModel> _benchmarkGroups;
		const BenchmarkModel* _selectedBenchmark;
	};

} // Application

#endif //ELPIDA_BENCHMARKSMODEL_HPP_
