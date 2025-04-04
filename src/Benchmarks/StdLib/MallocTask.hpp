//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef ELPIDA_MALLOCTASK_HPP
#define ELPIDA_MALLOCTASK_HPP

#include "Elpida/Core/IterationMicroTask.hpp"

#include <vector>

namespace Elpida
{

	class MallocTask : public IterationMicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		MallocTask();
		~MallocTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		void OnBeforeRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::vector<void*> _pointers;
		std::size_t _totalSize;
		std::vector<std::size_t> _sizes;
		SharedPtr<AbstractTaskData> _inputData;
	};

}

#endif //ELPIDA_MALLOCTASK_HPP
