/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
