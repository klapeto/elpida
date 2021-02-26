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

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_EXECUTIONPARAMETERS_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_EXECUTIONPARAMETERS_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class ExecutionParameters final
	{
	public:
		std::optional<size_t> getParameter(const std::string& name) const;
		void setParameter(const std::string& name, size_t value);

		ExecutionParameters() = default;
		~ExecutionParameters() =default;
	private:
		std::unordered_map<std::string, size_t> _parameterMap;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_EXECUTIONPARAMETERS_HPP
