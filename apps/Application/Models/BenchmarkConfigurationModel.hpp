//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_
#define ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_

#include "Models/Abstractions/Model.hpp"

#include <string>

namespace Elpida::Application
{

	enum class ConfigurationType
	{
		Integer,
		Float,
		String,
		File,
	};

	class BenchmarkConfigurationModel : public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetValue() const;

		[[nodiscard]]
		ConfigurationType GetType() const;

		void SetValue(std::string value);

		BenchmarkConfigurationModel(std::string name, std::string value, ConfigurationType type);
		BenchmarkConfigurationModel(const BenchmarkConfigurationModel&) = delete;
		BenchmarkConfigurationModel(BenchmarkConfigurationModel&&) noexcept = default;
		BenchmarkConfigurationModel& operator=(const BenchmarkConfigurationModel&) = delete;
		BenchmarkConfigurationModel& operator=(BenchmarkConfigurationModel&&) noexcept = default;
		~BenchmarkConfigurationModel() override = default;
	 private:
		std::string _name;
		std::string _value;
		ConfigurationType _type;
	};

} // Application

#endif //ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_