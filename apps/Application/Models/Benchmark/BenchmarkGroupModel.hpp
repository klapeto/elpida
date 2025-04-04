//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKGROUPMODEL_HPP_
#define ELPIDA_BENCHMARKGROUPMODEL_HPP_

#include "BenchmarkModel.hpp"

#include <string>

namespace Elpida::Application
{

	class BenchmarkGroupModel : public Model
	{
	 public:
		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::vector<BenchmarkModel>& GetBenchmarks() const;

		BenchmarkGroupModel(std::string name, std::vector<BenchmarkModel>&& benchmarks);
		BenchmarkGroupModel(const BenchmarkGroupModel&) = default;
		BenchmarkGroupModel(BenchmarkGroupModel&&) noexcept = default;
		BenchmarkGroupModel& operator=(const BenchmarkGroupModel&) = default;
		BenchmarkGroupModel& operator=(BenchmarkGroupModel&&) noexcept = default;
		~BenchmarkGroupModel() override = default;
	 private:
		std::string _name;
		std::vector<BenchmarkModel> _benchmarks;
	};

} // Application

#endif //ELPIDA_BENCHMARKGROUPMODEL_HPP_
