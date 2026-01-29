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

#ifndef _MEMORYREADBANDWIDTHTASK_HPP_
#define _MEMORYREADBANDWIDTHTASK_HPP_

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/IterationMicroTask.hpp"

namespace Elpida
{
	class MemoryReadBandwidthTask : public IterationMicroTask
	{
	 public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;
		explicit MemoryReadBandwidthTask();
		~MemoryReadBandwidthTask() override = default;
	 protected:
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
		UniquePtr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	 private:
		SharedPtr<AbstractTaskData> _data;
		unsigned char* _ptr;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHTASK_HPP_
