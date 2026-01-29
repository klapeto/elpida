/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkConfigurationModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkConfigurationModel::BenchmarkConfigurationModel(std::string name, std::string id, std::string value, ConfigurationType type)
		: _name(std::move(name)), _id(std::move(id)), _value(std::move(value)), _type(type)
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

	void BenchmarkConfigurationModel::SetValue(std::string value) const
	{
		_value = std::move(value);
		OnDataChanged();
	}

	const std::string& BenchmarkConfigurationModel::GetId() const
	{
		return _id;
	}
} // Application