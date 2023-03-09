//
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_THREADTASK_HPP_
#define ELPIDA_THREADTASK_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Elpida/UniquePtr.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/Duration.hpp"
#include "Elpida/Topology/TopologyNode.hpp"

namespace Elpida
{

	class ThreadTask final : public Task
	{
	 public:
		void Prepare(UniquePtr<AbstractTaskData> inputData) final;

		[[nodiscard]]
		Duration Run() final;

		[[nodiscard]]
		UniquePtr<AbstractTaskData> Finalize() final;

		[[nodiscard]]
		bool CanBeMultiThreaded() const final;

		[[nodiscard]]
		TaskInfo GetInfo() const final;

		void WakeThread();

		ThreadTask(UniquePtr<Task> taskToRun, Ref<const TopologyNode> targetProcessor);
		~ThreadTask() final;
	 protected:
		void DoRun() final;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const final;
	 private:
		UniquePtr<Task> _taskToRun;
		std::thread _thread;
		std::condition_variable _conditionVariable;
		std::mutex _mutex;
		Ref<const TopologyNode> _targetProcessor;
		Duration _taskRunDuration;
		volatile bool _doStart;

		void PinCurrentThreadToProcessor(const TopologyNode& topologyNode);
		void ThreadProcedure();
	};

} // Elpida

#endif //ELPIDA_THREADTASK_HPP_
