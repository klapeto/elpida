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
 * MemoryLatency.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_
#define TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_

#include <cstdint>
#include <string>
#include <random>

#include <Elpida/Engine/Task/MicroTask.hpp>
#include <Elpida/Utilities/NumaMemory.hpp>

namespace Elpida
{

	class ServiceProvider;

	class MemoryLatency : public MicroTask
	{
	public:
		explicit MemoryLatency(const TaskSpecification& specification,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t memorySize,
			size_t iterationsToRun);

		~MemoryLatency() override = default;
	protected:
		void prepareImpl() override;

		void run(size_t iterations) override;

		[[nodiscard]]
		double calculateTaskResultValue(const Duration& taskElapsedTime) const override;

		[[nodiscard]]
		double getInputDataSize() const override;

		[[nodiscard]]
		size_t getMinimumIterations() const override;
	private:
		NumaMemory _memory;
		const size_t MinimumIterations = 1000;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_ */
