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

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP

#include <type_traits>
#include <string>

namespace Elpida
{
	class ConfigurationSpecificationBase;

	/**
	 * Base class for all Configuration values
	 */
	class ConfigurationValueBase
	{
	public:

		[[nodiscard]] const ConfigurationSpecificationBase& getConfigurationSpecification() const
		{
			return _configurationSpecification;
		}

		[[nodiscard]] bool isReadOnly() const
		{
			return _readOnly;
		}

		[[nodiscard]] virtual ConfigurationValueBase* clone() const = 0;

		template<typename T>
		T& as()
		{
			static_assert(std::is_base_of_v<ConfigurationValueBase, T>,
				"Only derived from ConfigurationValueBase can be the template parameter");
			return static_cast<T&>(*this);
		}

		virtual ~ConfigurationValueBase() = default;
	protected:
		explicit ConfigurationValueBase(const ConfigurationSpecificationBase& configurationSpecification, bool readOnly)
			: _configurationSpecification(configurationSpecification), _readOnly(readOnly)
		{
		}

		const ConfigurationSpecificationBase& _configurationSpecification;
	private:
		bool _readOnly;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
