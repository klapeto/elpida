//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP

namespace Elpida
{
	class Task;
	class TaskBuilder;

	class BenchmarkTaskInstance final
	{
	public:

		[[nodiscard]] Task& getTask() const
		{
			return *_task;
		}

		[[nodiscard]] const TaskBuilder& getTaskBuilder() const
		{
			return *_taskBuilder;
		}

		BenchmarkTaskInstance(Task& task, TaskBuilder& taskBuilder);
		BenchmarkTaskInstance(BenchmarkTaskInstance&& other) noexcept;
		BenchmarkTaskInstance& operator=(BenchmarkTaskInstance&& other) noexcept;
		BenchmarkTaskInstance(const BenchmarkTaskInstance&) = delete;
		BenchmarkTaskInstance& operator=(const BenchmarkTaskInstance&) = delete;
		~BenchmarkTaskInstance();
	private:
		Task* _task;
		TaskBuilder* _taskBuilder;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP
