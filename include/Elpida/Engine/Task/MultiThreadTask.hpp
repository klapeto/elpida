//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP

#include "Task.hpp"
#include "TaskThread.hpp"

namespace Elpida {
	class Benchmark;

	class MultiThreadTask: public Task
	{
	public:
		void prepare() override;
		void finalize() override;
		void run() override;
		void applyAffinity() override;

		MultiThreadTask(const TaskSpecification& specification,
			TaskAffinity&& affinity,
			const Benchmark& singleThreadBenchmark,
			bool toBeCountedOnResults);
	private:
		std::mutex _mutex;
		std::condition_variable _wakeNotifier;
		std::vector<TaskSpecification*> _singleThreadSpecifications;
		std::vector<TaskThread> _createdThreads;
		const Benchmark& _singleThreadBenchmark;
		bool _threadsShouldWake;
	};
}



#endif //INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
