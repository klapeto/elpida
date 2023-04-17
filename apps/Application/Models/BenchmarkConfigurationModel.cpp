//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkConfigurationModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkConfigurationModel::BenchmarkConfigurationModel(std::string name, std::string value, ConfigurationType type)
		: _name(std::move(name)), _value(std::move(value)), _type(type)
	{
	}

	const std::string& BenchmarkConfigurationModel::GetName() const
	{
		return _name;
	}

	const std::string& BenchmarkConfigurationModel::GetValue() const
	{
		return _value;
	}

	ConfigurationType BenchmarkConfigurationModel::GetType() const
	{
		return _type;
	}

	void BenchmarkConfigurationModel::SetValue(std::string value)
	{
		_value = std::move(value);
		OnDataChanged();
	}
} // Application