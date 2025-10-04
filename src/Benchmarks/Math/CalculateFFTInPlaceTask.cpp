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

#include "CalculateFFTInPlaceTask.hpp"
#include "FFTCalculator.hpp"

#include <random>

namespace Elpida
{
	CalculateFFTInPlaceTask::CalculateFFTInPlaceTask(std::size_t size)
			:_size(size)
	{

	}

	void CalculateFFTInPlaceTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		auto totalValues = _size / sizeof(std::complex<Float>);
		if (totalValues < 2) totalValues = 2;
		totalValues = std::pow(std::floor(std::sqrt(totalValues)), 2);

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());

		std::normal_distribution distribution(0.0, 1.0);

		_values.resize(totalValues);
		for (std::size_t i = 0; i < totalValues; ++i)
		{
			_values[i] = std::complex<Float>(distribution(generator), distribution(generator));
		}

		_output = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> CalculateFFTInPlaceTask::Finalize()
	{
		return std::move(_output);
	}

	Size CalculateFFTInPlaceTask::GetProcessedDataSize() const
	{
		return _values.size();
	}

	TaskInfo CalculateFFTInPlaceTask::DoGetInfo() const
	{
		return {
				"Calculate FFT in place",
				"Calculates the DTF of a series of complex numbers",
				"numbers",
				"The rate of number processing",
				ResultType::Throughput };
	}

	UniquePtr<Task> CalculateFFTInPlaceTask::DoDuplicate() const
	{
		return std::make_unique<CalculateFFTInPlaceTask>(_size);
	}

	void CalculateFFTInPlaceTask::DoRunImpl()
	{
		Exec([this]()
		{
			FFTCalculator::CalculateFFTInPlace(_values);
		});
	}

	Size CalculateFFTInPlaceTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida