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

#ifndef ELPIDA_TASKRESULT_HPP_
#define ELPIDA_TASKRESULT_HPP_

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{

	class TaskResult final
	{
	 public:
		[[nodiscard]]
		const Duration& GetDuration() const
		{
			return _duration;
		}

		[[nodiscard]]
		Size GetDataSize() const
		{
			return _dataSize;
		}

		TaskResult(const Duration& duration, Size dataSize)
			: _duration(duration), _dataSize(dataSize)
		{
		}

		TaskResult(const TaskResult&) = delete;
		TaskResult(TaskResult&&) = default;
		~TaskResult() = default;
	 private:
		Duration _duration;
		Size _dataSize;
	};

} // Elpida

#endif //ELPIDA_TASKRESULT_HPP_
