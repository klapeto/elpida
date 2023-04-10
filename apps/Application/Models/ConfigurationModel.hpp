//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_CONFIGURATIONMODEL_HPP_
#define ELPIDA_CONFIGURATIONMODEL_HPP_

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

	class ConfigurationModel : public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetValue() const;

		[[nodiscard]]
		ConfigurationType GetType() const;

		void SetValue(std::string value);

		ConfigurationModel(std::string name, std::string value, ConfigurationType type);
		ConfigurationModel(const ConfigurationModel&) = delete;
		ConfigurationModel(ConfigurationModel&&) noexcept = default;
		ConfigurationModel& operator=(const ConfigurationModel&) = delete;
		ConfigurationModel& operator=(ConfigurationModel&&) noexcept = default;
		~ConfigurationModel() override = default;
	 private:
		std::string _name;
		std::string _value;
		ConfigurationType _type;
	};

} // Application

#endif //ELPIDA_CONFIGURATIONMODEL_HPP_
