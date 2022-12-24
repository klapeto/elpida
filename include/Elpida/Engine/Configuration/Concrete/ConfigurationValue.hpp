/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP

#include "ConfigurationValueBase.hpp"
#include <string>

namespace Elpida
{
	/**
	 * Type-Specific Configuration Value
	 * @tparam T Underlying type that will be used. The type must be compatible with the Type
	 */
	template<typename T>
	class ConfigurationValue : public ConfigurationValueBase
	{
	public:

		const T& getValue() const
		{
			return _value;
		}

		[[nodiscard]]
		std::unique_ptr<ConfigurationValueBase> clone() const override
		{
			return std::make_unique<ConfigurationValue<T>>(_configurationSpecification, (const T&)_value, isReadOnly());
		}

		void setValue(T value)
		{
			_value = value;
		}

		ConfigurationValue(const ConfigurationValue<T>&) = delete;
		ConfigurationValue<T>& operator=(const ConfigurationValue<T>&) = delete;

		explicit ConfigurationValue(const ConfigurationSpecificationBase& specification, T value, bool readOnly = false)
			: ConfigurationValueBase(specification, readOnly), _value(value)
		{

		}
		~ConfigurationValue() override = default;
	private:
		T _value;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
