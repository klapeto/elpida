//
// Created by klapeto on 22/8/2023.
//

#ifndef ELPIDA_TASKRESULTMODEL_HPP
#define ELPIDA_TASKRESULTMODEL_HPP

#include "Abstractions/Model.hpp"
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
		TaskResultModel(const TaskResultModel&) = delete;
		TaskResultModel& operator=(const TaskResultModel&) = delete;
		TaskResultModel& operator=(TaskResultModel&&) noexcept = default;
		~TaskResultModel() override = default;
	private:
		Duration _duration;
		std::size_t _inputSize;
	};

} // Application

#endif //ELPIDA_TASKRESULTMODEL_HPP
