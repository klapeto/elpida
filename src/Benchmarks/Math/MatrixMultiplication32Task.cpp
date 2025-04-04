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

#include "MatrixMultiplication32Task.hpp"

#include <random>

namespace Elpida
{

	void MatrixMultiplication32Task::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());

		std::normal_distribution distribution(0.0, 100.0);

		for (std::size_t i = 0; i < _matrix.GetRows(); ++i)
		{
			for (std::size_t j = 0; j < _matrix.GetColumns(); ++j)
			{
				_a.Get(i, j) = distribution(generator);
				_b.Get(i, j) = distribution(generator);
			}
		}
	}

	SharedPtr<AbstractTaskData> MatrixMultiplication32Task::Finalize()
	{
		return std::move(_inputData);
	}

	Size MatrixMultiplication32Task::GetProcessedDataSize() const
	{
		return _matrix.GetColumns() * _matrix.GetRows() * 2;
	}

	void MatrixMultiplication32Task::DoRunImpl()
	{
		Exec([this]()
		{
			_matrix = _a * _b;
		});
	}

	Size MatrixMultiplication32Task::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> MatrixMultiplication32Task::DoDuplicate() const
	{
		return std::make_unique<MatrixMultiplication32Task>();
	}

	TaskInfo MatrixMultiplication32Task::DoGetInfo() const
	{
		return { "Matrix multiplication (32x32)",
				 "Calculates the product between 2 32x23 matrices",
				 "Numbers",
				 "The amount of numbers processed per second",
				 ResultType::Throughput
		};
	}

}