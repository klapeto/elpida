/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * MemoryReadCached.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_

#include <Elpida/Engine/Task/MicroTask.hpp>
#include <Elpida/Utilities/NumaMemory.hpp>

namespace Elpida
{
	class TaskMetrics;
	class ServiceProvider;

	class MemoryReadBandwidth final : public MicroTask
	{
	public:

		MemoryReadBandwidth(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t memorySize,
			size_t iterationsToRun);
		~MemoryReadBandwidth() override = default;
	protected:
		NumaMemory _memory;
		const ServiceProvider& _serviceProvider;

		void run(size_t iterations) override;
		void prepareImpl() override;

		[[nodiscard]]
		double getInputDataSize() const override;

		[[nodiscard]]
		Duration getMinimumExecutionTime() const override;

		[[nodiscard]]
		double calculateTaskResultValue(const Duration& taskElapsedTime) const override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREAD_HPP_ */
