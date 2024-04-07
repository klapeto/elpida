//
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_THREADTASK_HPP_
#define ELPIDA_THREADTASK_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Topology/TopologyNode.hpp"

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
		Size GetProcessedDataSize() const override;

		void WakeThread();

		ThreadTask(UniquePtr<Task> taskToRun, Ref<const ProcessingUnitNode> targetProcessor, bool pin);
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
		Ref<const ProcessingUnitNode> _targetProcessor;
		Duration _taskRunDuration;
		volatile bool _doStart;
		bool _pin;

		void ThreadProcedure();

		[[nodiscard]]
		TaskInfo DoGetInfo() const final;
	};

} // Elpida

#endif //ELPIDA_THREADTASK_HPP_
