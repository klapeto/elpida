/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 22/2/21.
//

#include "Elpida/Engine/Task/ExecutionParameters.hpp"


namespace Elpida
{

	std::optional<size_t> ExecutionParameters::getParameter(const std::string& name) const
	{
		auto itr = _parameterMap.find(name);
		if (itr != _parameterMap.end())
		{
			return itr->second;
		}

		return std::nullopt;
	}
	void ExecutionParameters::setParameter(const std::string& name, size_t value)
	{
		_parameterMap.emplace(name, value);
	}
}