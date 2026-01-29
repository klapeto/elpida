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
// Created by klapeto on 3/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP
#define ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/ResultType.hpp"

namespace Elpida::Application
{

	class TaskModel final : public Model
	{
	public:

		const std::string& GetName() const
		{
			return _name;
		}

		const std::string& GetUnit() const
		{
			return _unit;
		}

		ResultType GetResultType() const
		{
			return _resultType;
		}

		bool IsMeasured() const
		{
			return _isMeasured;
		}

		TaskModel(std::string name, std::string unit, ResultType resultType, bool isMeasured)
				:_name(std::move(name)), _unit(std::move(unit)), _resultType(resultType), _isMeasured(isMeasured)
		{

		}

		TaskModel(const TaskModel&) = default;
		TaskModel(TaskModel&&) noexcept = default;
		TaskModel& operator=(const TaskModel&) = default;
		TaskModel& operator=(TaskModel&&) noexcept = default;
		~TaskModel() final = default;
	private:
		std::string _name;
		std::string _unit;
		ResultType _resultType;
		bool _isMeasured;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP
