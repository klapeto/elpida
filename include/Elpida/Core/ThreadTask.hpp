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
	class TopologyInfo;

	class ThreadTask final : public Task
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) final;

		[[nodiscard]]
		Duration Run() final;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() final;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		void WakeThread();

		ThreadTask(UniquePtr<Task> taskToRun, Optional<Ref<const ProcessingUnitNode>> targetProcessor, const TopologyInfo& topologyInfo);
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
		const TopologyInfo& _topologyInfo;
		Optional<Ref<const ProcessingUnitNode>> _targetProcessor;
		Duration _taskRunDuration;
		std::exception_ptr _exception;
		volatile bool _doStart;

		void ThreadProcedure();

		[[nodiscard]]
		TaskInfo DoGetInfo() const final;
	};

} // Elpida

#endif //ELPIDA_THREADTASK_HPP_
