//
// Created by klapeto on 3/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP
#define ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/ScoreType.hpp"

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

		ScoreType GetScoreType() const
		{
			return _scoreType;
		}

		TaskModel(std::string name, std::string unit, ScoreType scoreType)
			: _name(std::move(name)), _unit(std::move(unit)), _scoreType(scoreType)
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
		ScoreType _scoreType;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_MODELS_TASKMODEL_HPP
