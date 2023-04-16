//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_BENCHMARKCONFIGURATIONINSTANCEMODEL_HPP_
#define ELPIDA_BENCHMARKCONFIGURATIONINSTANCEMODEL_HPP_

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

	class BenchmarkConfigurationInstanceModel : public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetValue() const;

		[[nodiscard]]
		ConfigurationType GetType() const;

		void SetValue(std::string value);

		BenchmarkConfigurationInstanceModel(std::string name, std::string value, ConfigurationType type);
		BenchmarkConfigurationInstanceModel(const BenchmarkConfigurationInstanceModel&) = delete;
		BenchmarkConfigurationInstanceModel(BenchmarkConfigurationInstanceModel&&) noexcept = default;
		BenchmarkConfigurationInstanceModel& operator=(const BenchmarkConfigurationInstanceModel&) = delete;
		BenchmarkConfigurationInstanceModel& operator=(BenchmarkConfigurationInstanceModel&&) noexcept = default;
		~BenchmarkConfigurationInstanceModel() override = default;
	 private:
		std::string _name;
		std::string _value;
		ConfigurationType _type;
	};

} // Application

#endif //ELPIDA_BENCHMARKCONFIGURATIONINSTANCEMODEL_HPP_
