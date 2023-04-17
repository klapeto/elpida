//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKMODEL_HPP_
#define ELPIDA_BENCHMARKMODEL_HPP_

#include "Models/Abstractions/Model.hpp"
#include "BenchmarkConfigurationModel.hpp"

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
		const std::string& GetFilePath() const;

		[[nodiscard]]
		std::size_t GetIndex() const;

		[[nodiscard]]
		const std::vector<BenchmarkConfigurationModel>& GetConfigurations() const;

		BenchmarkModel(std::string name, std::string filePath, std::size_t index, std::vector<BenchmarkConfigurationModel>&& configurations);
		BenchmarkModel(const BenchmarkModel&) = delete;
		BenchmarkModel(BenchmarkModel&&) noexcept = default;
		BenchmarkModel& operator=(const BenchmarkModel&) = delete;
		BenchmarkModel& operator=(BenchmarkModel&&) noexcept = default;
		~BenchmarkModel() override = default;
	 private:
		std::string _name;
		std::string _filePath;
		std::vector<BenchmarkConfigurationModel> _configurations;
		std::size_t _index;
	};

} // Application

#endif //ELPIDA_BENCHMARKMODEL_HPP_
