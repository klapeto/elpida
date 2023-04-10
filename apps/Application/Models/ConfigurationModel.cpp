//
// Created by klapeto on 8/4/2023.
//

#include "ConfigurationModel.hpp"

#include <utility>

namespace Elpida::Application
{
	ConfigurationModel::ConfigurationModel(std::string name, std::string value, ConfigurationType type)
		: _name(std::move(name)), _value(std::move(value)), _type(type)
	{
	}
	const std::string& ConfigurationModel::GetName() const
	{
		return _name;
	}
	const std::string& ConfigurationModel::GetValue() const
	{
		return _value;
	}
	ConfigurationType ConfigurationModel::GetType() const
	{
		return _type;
	}

	void ConfigurationModel::SetValue(std::string value)
	{
		_value = std::move(value);
		OnDataChanged();
	}
} // Application