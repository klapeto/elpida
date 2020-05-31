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
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP

namespace Elpida
{
	class ConfigurationSpecificationBase;
	class ConfigurationValueBase;

	class ConfigurationInstance
	{
	public:
		[[nodiscard]] const ConfigurationSpecificationBase& getConfigurationSpecification() const
		{
			return *_configurationSpecification;
		}

		[[nodiscard]] const ConfigurationValueBase* getValue() const
		{
			return _value;
		}

		[[nodiscard]] bool isFixed() const
		{
			return _fixed;
		}

		explicit ConfigurationInstance(const ConfigurationSpecificationBase& configurationSpecification,
			const ConfigurationValueBase* value, bool fixed)
			: _configurationSpecification(&configurationSpecification), _value(value), _fixed(fixed)
		{

		}
		ConfigurationInstance(ConfigurationInstance&& other) noexcept;
		ConfigurationInstance& operator=(ConfigurationInstance&& other) noexcept;
		virtual ~ConfigurationInstance();
	private:
		const ConfigurationSpecificationBase* _configurationSpecification;
		const ConfigurationValueBase* _value;
		bool _fixed;
		void move(ConfigurationInstance&& other) noexcept;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONINSTANCE_HPP
