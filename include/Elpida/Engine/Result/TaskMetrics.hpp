//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP

#include <chrono>

namespace Elpida
{
	class TaskMetrics final
	{
	public:

		typedef std::nano NanoSecond;
		typedef std::micro MicroSecond;
		typedef std::milli MilliSecond;
		typedef std::ratio<1, 1> Second;
		typedef std::chrono::duration<double> Duration;

		[[nodiscard]] const Duration& getDuration() const
		{
			return _duration;
		}

		[[nodiscard]] size_t getInputDataSize() const
		{
			return _inputDataSize;
		}
		[[nodiscard]] size_t getActualProcessDataSize() const
		{
			return _actualProcessDataSize;
		}

		template<typename Division>
		[[nodiscard]] double getDurationSubdivision() const
		{
			return ((double)Division::den * _duration.count()) / (double)Division::num;
		}

		explicit TaskMetrics(const Duration& duration, size_t inputDataSize, size_t actualProcessDataSize)
			: _duration(duration), _inputDataSize(inputDataSize), _actualProcessDataSize(actualProcessDataSize)
		{

		}

		TaskMetrics(TaskMetrics&&) = default;
		TaskMetrics(const TaskMetrics&) = default;
		TaskMetrics& operator=(TaskMetrics&&) = default;
		TaskMetrics& operator=(const TaskMetrics&) = default;
		~TaskMetrics() = default;
	private:
		Duration _duration;
		size_t _inputDataSize;
		size_t _actualProcessDataSize;
	};

}
#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKMETRICS_HPP
