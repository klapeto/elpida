//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP

#include <chrono>
#include "Elpida/Utilities/Duration.hpp"

namespace Elpida
{
	class TaskMetrics final
	{
	public:

		[[nodiscard]] const Duration& getDuration() const
		{
			return _duration;
		}

		[[nodiscard]] double getResultValue() const
		{
			return _resultValue;
		}

		[[nodiscard]] size_t getInputDataSize() const
		{
			return _inputDataSize;
		}

		template<typename Division>
		[[nodiscard]] double getDurationSubdivision() const
		{
			return ((double)Division::den * _duration.count()) / (double)Division::num;
		}

		explicit TaskMetrics(const Elpida::Duration& duration, double resultValue, size_t inputDataSize);
		TaskMetrics(TaskMetrics&&) = default;
		TaskMetrics(const TaskMetrics&) = default;
		TaskMetrics& operator=(TaskMetrics&&) = default;
		TaskMetrics& operator=(const TaskMetrics&) = default;
		~TaskMetrics() = default;
	private:
		Duration _duration;
		double _resultValue;
		size_t _inputDataSize;
	};

}
#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
