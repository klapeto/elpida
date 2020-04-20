//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Engine/Benchmark.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskSpecification& specification,
		TaskAffinity&& affinity,
		const Benchmark& singleThreadBenchmark,
		bool toBeCountedOnResults)
		: Task(specification, std::move(affinity), toBeCountedOnResults),
		  _singleThreadBenchmark(singleThreadBenchmark), _threadsShouldWake(false)
	{
	}

	void MultiThreadTask::prepare()
	{
		for (auto processor : _affinity.getProcessorNodes())
		{
			auto tasks =
				_singleThreadBenchmark.createTasks(TaskAffinity(std::vector<const ProcessorNode*>{ processor }));
			for (auto task :tasks)
			{
				task->prepare();
			}
			auto thread =
				TaskThread(std::move(tasks), _wakeNotifier, _mutex, _threadsShouldWake, processor->getOsIndex());
			thread.start();
			_createdThreads.push_back(std::move(thread));
		}
	}

	void MultiThreadTask::finalize()
	{
		for (auto& thread: _createdThreads)
		{
			for (auto task: thread.getTasksToRun())
			{
				task->finalize();
			}
		}

		_createdThreads.clear();
	}
	void MultiThreadTask::run()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_threadsShouldWake = true;
		lock.unlock();
		_wakeNotifier.notify_all();
		for (auto& thread : _createdThreads)
		{
			thread.join();
		}
	}

	void MultiThreadTask::applyAffinity()
	{

	}
}