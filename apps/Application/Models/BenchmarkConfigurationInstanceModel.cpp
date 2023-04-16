//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkConfigurationInstanceModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkConfigurationInstanceModel::BenchmarkConfigurationInstanceModel(std::string name, std::string value, ConfigurationType type)
		: _name(std::move(name)), _value(std::move(value)), _type(type)
	{
	}

	const std::string& BenchmarkConfigurationInstanceModel::GetName() const
	{
		return _name;
	}

	const std::string& BenchmarkConfigurationInstanceModel::GetValue() const
	{
		return _value;
	}

	ConfigurationType BenchmarkConfigurationInstanceModel::GetType() const
	{
		return _type;
	}

	void BenchmarkConfigurationInstanceModel::SetValue(std::string value)
	{
		_value = std::move(value);
		OnDataChanged();
	}
} // Application