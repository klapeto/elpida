//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKMODEL_HPP_
#define ELPIDA_BENCHMARKMODEL_HPP_

#include "Models/Abstractions/Model.hpp"
#include "BenchmarkConfigurationModel.hpp"
#include "TaskModel.hpp"

#include <vector>
#include <cstdlib>

namespace Elpida::Application
{

	class BenchmarkModel : public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetDescription() const;

		[[nodiscard]]
		const std::string& GetFilePath() const;

		[[nodiscard]]
		std::size_t GetBenchmarkIndex() const;

		[[nodiscard]]
		const std::string& GetResultUnit() const;

		[[nodiscard]]
		ResultType GetResultType() const;

		[[nodiscard]]
		const std::vector<BenchmarkConfigurationModel>& GetConfigurations() const;

		BenchmarkModel(
				std::string name,
				std::string description,
				std::string resultUnit,
				ResultType resultType,
				std::string filePath,
				std::size_t benchmarkIndex,
				std::vector<BenchmarkConfigurationModel>&& configurations);
		BenchmarkModel(const BenchmarkModel&) = default;
		BenchmarkModel(BenchmarkModel&&) noexcept = default;
		BenchmarkModel& operator=(const BenchmarkModel&) = default;
		BenchmarkModel& operator=(BenchmarkModel&&) noexcept = default;
		~BenchmarkModel() override = default;
	 private:
		std::string _name;
		std::string _description;
		std::string _filePath;
		std::string _resultUnit;
		std::vector<BenchmarkConfigurationModel> _configurations;
		std::size_t _benchmarkIndex;
		ResultType _resultType;
	};

} // Application

#endif //ELPIDA_BENCHMARKMODEL_HPP_
