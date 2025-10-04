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

#include "DynamicMatrixMultiplicationTask.hpp"

#include <random>

namespace Elpida
{
	DynamicMatrixMultiplicationTask::DynamicMatrixMultiplicationTask(std::size_t rows, std::size_t columns)
			:_rows(rows), _columns(columns)
	{

	}

	void DynamicMatrixMultiplicationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output = std::move(inputData);

		_a = DynamicMatrix<Float>(_rows, _columns);
		_b = DynamicMatrix<Float>(_columns, _rows);

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());

		std::normal_distribution distribution(0.0, 100.0);

		for (std::size_t i = 0; i < _rows; ++i)
		{
			for (std::size_t j = 0; j < _columns; ++j)
			{
				_a.Get(i, j) = distribution(generator);
				_b.Get(i, j) = distribution(generator);
			}
		}
	}

	SharedPtr<AbstractTaskData> DynamicMatrixMultiplicationTask::Finalize()
	{
		return std::move(_output);
	}

	Size DynamicMatrixMultiplicationTask::GetProcessedDataSize() const
	{
		return _rows * _columns * 2;
	}

	TaskInfo DynamicMatrixMultiplicationTask::DoGetInfo() const
	{
		return {
				"Dynamic matrix multiplication",
				"Multiplies 2 matrices that their dimensions are not known in advanced",
				"numbers",
				"The amount of numbers processed per second",
				ResultType::Throughput };
	}

	UniquePtr<Task> DynamicMatrixMultiplicationTask::DoDuplicate() const
	{
		return std::make_unique<DynamicMatrixMultiplicationTask>(_rows, _columns);
	}

	void DynamicMatrixMultiplicationTask::DoRunImpl()
	{
		Exec([this]()
		{
			_matrix = _a.Multiply(_b);
		});
	}

	Size DynamicMatrixMultiplicationTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida