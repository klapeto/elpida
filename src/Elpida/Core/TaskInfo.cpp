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
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/TaskInfo.hpp"

#include <utility>

namespace Elpida
{
	TaskInfo::TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ResultType resultType) :_name(std::move(name)),
									_description(std::move(description)),
									_resultUnit(std::move(scoreUnit)),
									_resultDescription(
									   std::move(scoreDescription)),
									_resultType(resultType),
									_isMeasured(false)
	{
	}

	TaskInfo::TaskInfo(String name,
			String description,
			String scoreUnit,
			String scoreDescription,
			ResultType scoreType,
			bool isMeasured) :_name(std::move(name)),
							  _description(std::move(description)),
							  _resultUnit(std::move(scoreUnit)),
							  _resultDescription(
									   std::move(scoreDescription)),
							  _resultType(scoreType),
							  _isMeasured(isMeasured)
	{
	}
} // Elpida