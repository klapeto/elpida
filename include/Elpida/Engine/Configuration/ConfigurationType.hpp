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
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP

#include <string>
#include <functional>

namespace Elpida
{
	class ConfigurationType
	{
	public:

		using Int = long;
		using UnsignedInt = unsigned long;
		using Float = double;
		using String = std::string;
		using Bool = bool;
		using FilePath = std::string;
		using FolderPath = std::string;
		using Custom = void*;

		template<typename T>
		using Function = std::function<T>;

		enum class Type
		{
			String,
			Bool,
			Int,
			UnsignedInt,
			Float,
			FilePath,
			FolderPath,
			Function,
			Custom,
		};

		ConfigurationType() = delete;
		~ConfigurationType() = delete;
	};

}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
