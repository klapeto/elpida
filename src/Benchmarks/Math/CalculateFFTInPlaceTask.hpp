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
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_CALCULATEFFTINPLACETASK_HPP
#define ELPIDA_CALCULATEFFTINPLACETASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Core/Float.hpp"
#include <vector>
#include <complex>

namespace Elpida
{

	class CalculateFFTInPlaceTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit CalculateFFTInPlaceTask(std::size_t size);
		CalculateFFTInPlaceTask(const CalculateFFTInPlaceTask&) = delete;
		CalculateFFTInPlaceTask(CalculateFFTInPlaceTask&&) noexcept = delete;
		CalculateFFTInPlaceTask& operator=(CalculateFFTInPlaceTask&&) noexcept = delete;
		CalculateFFTInPlaceTask& operator=(const CalculateFFTInPlaceTask&) = delete;
		~CalculateFFTInPlaceTask() override = default;
	private:
		std::vector<std::complex<Float>> _values;
		SharedPtr<AbstractTaskData> _output;
		std::size_t _size;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRunImpl() override;
		Size GetOperationsPerformedPerRun() override;
	};
} // Elpida

#endif //ELPIDA_CALCULATEFFTINPLACETASK_HPP
