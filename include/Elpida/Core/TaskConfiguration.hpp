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
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TASKCONFIGURATION_HPP_
#define ELPIDA_TASKCONFIGURATION_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{

	enum class ConfigurationType
	{
		Integer,
		Float,
		String,
		File,
		Boolean,
	};

	class TaskConfiguration final
	{
	 public:
		using File = std::string;
		using Integer = long;
		using Float = double;
		using String = std::string;
		using Boolean = bool;

		[[nodiscard]]
		const String& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const String& GetValue() const
		{
			return _value;
		}

		[[nodiscard]]
		ConfigurationType GetType() const
		{
			return _type;
		}

		void SetValue(Integer value);
		void SetValue(const String& value);
		void SetValue(Float value);
		void SetValue(Boolean value);
		void Parse(const String& value);

		Integer AsInteger() const;
		Float AsFloat() const;
		Boolean AsBoolean() const;

		TaskConfiguration(String name, ConfigurationType type, String defaultValue = String());
		TaskConfiguration(const TaskConfiguration&) = default;
		TaskConfiguration(TaskConfiguration&&) noexcept = default;
		TaskConfiguration& operator=(const TaskConfiguration&) = default;
		TaskConfiguration& operator=(TaskConfiguration&&) noexcept = default;
		~TaskConfiguration() = default;
	 private:
		String _name;
		String _value;
		ConfigurationType _type;
	};

} // Elpida

#endif //ELPIDA_TASKCONFIGURATION_HPP_
