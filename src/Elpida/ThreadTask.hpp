//
// Created by klapeto on 28/2/2023.
//

#ifndef _THREADTASK_HPP_
#define _THREADTASK_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Task.hpp"
#include "Duration.hpp"
#include "Elpida/Topology/TopologyNode.hpp"

namespace Elpida
{

	class ThreadTask final : public Task
	{
	 public:
		void Prepare(TaskData&& inputData) final;
		Duration Run() final;
		TaskData Finalize() final;
		bool CanBeMultiThreaded() const final;
		TaskInfo GetInfo() const final;

		void WakeThread();

		ThreadTask(std::unique_ptr<Task> taskToRun, std::reference_wrapper<const TopologyNode> targetProcessor);
		~ThreadTask() final;
	 protected:
		void DoRun() final;
		std::unique_ptr<Task> DoDuplicate() const final;
	 private:
		std::unique_ptr<Task> _taskToRun;
		std::thread _thread;
		std::condition_variable _conditionVariable;
		std::mutex _mutex;
		std::reference_wrapper<const TopologyNode> _targetProcessor;
		Duration _taskRunDuration;
		volatile bool _doStart;

		void PinCurrentThreadToProcessor(const TopologyNode& topologyNode);
		void ThreadProcedure();
	};

} // Elpida

#endif //_THREADTASK_HPP_
