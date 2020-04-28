//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP

#include "Task.hpp"
#include "TaskThread.hpp"

namespace Elpida
{
	class TaskConfiguration;

	class MultiThreadTask : public Task
	{
	public:
		void execute() override;
		void applyAffinity() override;

		MultiThreadTask(const TaskSpecification& taskSpecification, const TaskConfiguration& configuration, const TaskAffinity& affinity);
		~MultiThreadTask() override = default;

	protected:
		void prepareImpl() override;
		TaskOutput finalizeAndGetOutputData() override;
	private:
		std::mutex _mutex;
		std::condition_variable _wakeNotifier;
		std::vector<TaskThread> _createdThreads;
		const TaskConfiguration& _configuration;
		bool _threadsShouldWake;

	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_MULTITHREADTASK_HPP
