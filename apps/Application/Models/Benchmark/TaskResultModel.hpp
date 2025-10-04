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
// Created by klapeto on 22/8/2023.
//

#ifndef ELPIDA_TASKRESULTMODEL_HPP
#define ELPIDA_TASKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/Duration.hpp"

namespace Elpida::Application
{

	class TaskResultModel final : public Model
	{
	public:

		const Duration& GetDuration() const
		{
			return _duration;
		}

		std::size_t GetInputSize() const
		{
			return _inputSize;
		}

		TaskResultModel(Duration duration, std::size_t inputSize)
				: _duration(duration), _inputSize(inputSize)
		{
		}

		TaskResultModel(TaskResultModel&&) noexcept = default;
		TaskResultModel(const TaskResultModel&) = default;
		TaskResultModel& operator=(const TaskResultModel&) = default;
		TaskResultModel& operator=(TaskResultModel&&) noexcept = default;
		~TaskResultModel() override = default;
	private:
		Duration _duration;
		std::size_t _inputSize;
	};

} // Application

#endif //ELPIDA_TASKRESULTMODEL_HPP
